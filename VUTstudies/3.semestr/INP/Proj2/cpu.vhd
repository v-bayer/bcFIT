-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2014 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Vaclav Bayer
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	type instr_type is (i_noop, i_addr_inc, i_addr_dec, i_ptr_inc, i_ptr_dec, i_while_start, i_while_end, i_print, i_load, i_halt);

	type fsm_state is (s_start, s_load_instr1, s_load_instr2, s_ptr_inc, s_ptr_dec, s_load_value1 , s_load_value2, s_print, s_noop, s_halt,
						s_swhile_start1, s_swhile_start2, s_while_miss_forw1, s_while_miss_forw2, swhile_end2, swhile_end3, s_while_miss_back1, s_while_miss_back2);

	signal ptr_instr_type : instr_type;
	signal present_state : fsm_state;
	signal next_state : fsm_state;

	signal cnt_state : std_logic;
	signal cnt_increment : std_logic;
	signal cnt_decrement : std_logic;
	signal cnt_registr : std_logic_vector (7 downto 0);

	signal pc_state : std_logic;
	signal pc_increment : std_logic;
	signal pc_decrement : std_logic;
	signal pc_registr : std_logic_vector (12 downto 0);

	signal ptr_state : std_logic;
	signal ptr_increment : std_logic;
	signal ptr_decrement : std_logic;
	signal ptr_registr : std_logic_vector (12 downto 0);



begin
	-------------------------------------------------------------------------------------
	-- COUNTER FOR WHILES
	-------------------------------------------------------------------------------------
	reg_CNT : process (RESET, CLK)
	begin
		if(RESET = '1') then
			cnt_registr <= (others => '0');
		elsif(CLK'event) and (CLK = '1') then
			if(cnt_increment = '1') then
				cnt_registr <= cnt_registr + 1;
			elsif(cnt_decrement = '1') then
				cnt_registr <= cnt_registr - 1;
			elsif(cnt_state = '1') then
				cnt_registr <= "00000001";
			end if;
		end if;
	end process;

	-------------------------------------------------------------------------------------
	-- PROGRAM COUNTER 
	-------------------------------------------------------------------------------------
	reg_PC: process (RESET, CLK)
	begin
		if(RESET = '1') then
			pc_registr <= (others => '0');
		elsif(CLK'event) and (CLK = '1') then
			if(pc_increment = '1') then
				pc_registr <= pc_registr + 1;
			elsif(pc_decrement = '1') then
				pc_registr <= pc_registr - 1;
			end if;
		end if;
	end process;

	-------------------------------------------------------------------------------------
	-- POINTER COUNTER
	-------------------------------------------------------------------------------------
	reg_PTR: process (RESET, CLK)
	begin
		if(RESET = '1') then
			ptr_registr <= "1000000000000";
		elsif(CLK'event) and (CLK = '1') then
			if(ptr_increment = '1') then
				if(ptr_registr = "1111111111111") then
					ptr_registr <= "1000000000000";
				else
					ptr_registr <= ptr_registr + 1;
				end if;
			elsif(ptr_decrement = '1') then
				if(ptr_registr = "1000000000000") then
					ptr_registr <= "1111111111111";
				else
					ptr_registr <= ptr_registr - 1;
				end if;
			end if;
		end if;
	end process;

	-------------------------------------------------------------------------------------
	-- DECODE INSTRUCTION
	-------------------------------------------------------------------------------------
	nacteni_instrukce : process (DATA_RDATA)
	begin
		case DATA_RDATA is
			when X"3E" => ptr_instr_type <= i_addr_inc;		-- >
			when X"3C" => ptr_instr_type <= i_addr_dec; 	-- <
			when X"2B" => ptr_instr_type <= i_ptr_inc;		-- +
			when X"2D" => ptr_instr_type <= i_ptr_dec; 		-- - 
			when X"2E" => ptr_instr_type <= i_print; 		-- .
			when X"2C" => ptr_instr_type <= i_load; 		-- ,
			when X"5B" => ptr_instr_type <= i_while_start; 	-- [
			when X"5D" => ptr_instr_type <= i_while_end; 	-- ]
			when X"00" => ptr_instr_type <= i_halt; 		-- null
			when others => ptr_instr_type <= i_noop; 		-- others
		end case;
	end process;

	-------------------------------------------------------------------------------------
	-- MX1
	-------------------------------------------------------------------------------------
	DATA_ADDR <= pc_registr when pc_state = '1' else (others => 'Z');
	DATA_ADDR <= ptr_registr when ptr_state = '1' else (others => 'Z');

	-------------------------------------------------------------------------------------
	-- PRESENT STATE
	-------------------------------------------------------------------------------------
	present_state_registr : process (CLK, RESET, EN)
	begin
		if(RESET = '1') then
			present_state <= s_start;
		elsif(CLK'event) and (CLK = '1') then
			if (EN = '1') then
				present_state <= next_state;
			end if;		
		end if;
	end process;

	-------------------------------------------------------------------------------------
	-- NEXT STATE
	-------------------------------------------------------------------------------------
	next_state_registr : process (EN, present_state, IN_VLD, IN_DATA, DATA_RDATA, OUT_BUSY, ptr_instr_type)
	begin

		cnt_state <= '0';
		pc_state <= '0';
		ptr_state <= '0';

		cnt_increment <= '0';
		cnt_decrement <= '0';

		pc_increment <= '0';
		pc_decrement <= '0';

		ptr_increment <= '0';
		ptr_decrement <= '0';

		IN_REQ <= '0';
		OUT_WE <= '0';
		DATA_EN <= '0';
		DATA_WDATA <= X"00";
		DATA_RDWR <= '0';

		next_state <= s_start;

		case present_state is

			-- FIRST STATE
			when s_start =>
				next_state <= s_load_instr1;

			-- LOAD INSTRUCTION
			when s_load_instr1 =>
				pc_state <= '1';
				DATA_EN <= '1';
				next_state <= s_load_instr2;

			-- LOAD INSTRUCTION v druhem taktu(dekodovani)
			when s_load_instr2 => 
				case ptr_instr_type is

					-- >
					when i_addr_inc =>
						pc_increment <= '1';
						ptr_increment <= '1';
						next_state <= s_start;

					-- <
					when i_addr_dec =>
						pc_increment <= '1';
						ptr_decrement <= '1';
						next_state <= s_start;

					-- +
					when i_ptr_inc =>
						DATA_EN <= '1';
						ptr_state <= '1';
						DATA_RDWR <= '0';
						next_state <= s_ptr_inc;

					-- -
					when i_ptr_dec =>
						DATA_EN <= '1';
						ptr_state <= '1';
						DATA_RDWR <= '0';
						next_state <= s_ptr_dec;

					-- ,
					when i_load =>
						next_state <= s_load_value1;

					-- .
					when i_print =>
						DATA_EN <= '1';
						ptr_state <= '1';
						DATA_RDWR <= '0';
						next_state <= s_print;

					-- [
					when i_while_start =>
            			pc_increment <= '1';
            			ptr_state <= '1';
           				DATA_EN <= '1';
            			DATA_RDWR <= '0';
            			next_state <= s_swhile_start1;

            		-- ]
					when i_while_end =>
						ptr_state <= '1';
            			DATA_EN <= '1';
            			DATA_RDWR <= '0';
            			next_state <= swhile_end2;

            		-- halt
					when i_halt =>
						next_state <= s_halt;

					when i_noop =>
						next_state <= s_noop;
				end case;
			

			-- PTR++
			when s_ptr_inc =>
				DATA_EN <= '1';
				ptr_state <= '1';
				DATA_RDWR <= '1';
				DATA_WDATA <= DATA_RDATA + 1;
				pc_increment <= '1';
				next_state <= s_start;

			-- PTR--
			when s_ptr_dec =>
				DATA_EN <= '1';
				DATA_RDWR <= '1';
				ptr_state <= '1';
				DATA_WDATA <= DATA_RDATA - 1;
				pc_increment <= '1';
				next_state <= s_start;

			-- REQ LOAD VALUE
			when s_load_value1 =>
				IN_REQ <= '1';
				next_state <= s_load_value2;

			-- LOAD VALUE
			when s_load_value2 =>
				IN_REQ <= '1';
				if(IN_VLD = '1') then
					DATA_EN <= '1';
					ptr_state <= '1';
					DATA_RDWR <= '1';
					DATA_WDATA <= IN_DATA;
				        pc_increment <= '1';
			       		next_state <= s_start;
			 		IN_REQ <= '0';
				else
					next_state <= s_load_value1;
				end if;		

			-- PRINT
			when s_print =>
				if(OUT_BUSY = '0') then
					OUT_WE <= '1';
					OUT_DATA <= DATA_RDATA;
					pc_increment <= '1';
					next_state <= s_start;
				else
					next_state <= s_print;
				end if;

			-- IF NULL -> HALT
			when s_halt =>
				next_state <= s_halt;

			-- SKIP
			when s_noop =>
				pc_increment <= '1';
				next_state <= s_start;


		----------- WHILE -----------
		-- WHILE PTR takt 2
		when s_swhile_start1 =>
			if ( DATA_RDATA = X"00" ) then
				cnt_increment <= '1';
				next_state <= s_swhile_start2;        
			else 
				next_state <= s_start;
			end if;

		-- WHILE 0 (bez nacteni)
		when s_swhile_start2 =>
			next_state <= s_while_miss_forw1;
			pc_state <= '1';
			DATA_EN <= '1';

		-- WHILE 0 (bez dokodovani)
		when s_while_miss_forw1 =>
			pc_increment <= '1';
			next_state <= s_while_miss_forw2;     
			if ( ptr_instr_type = i_while_start ) then
				cnt_increment <= '1';
			elsif ( ptr_instr_type = i_while_end ) then
				cnt_decrement <= '1';
			end if;

		when s_while_miss_forw2 =>
			next_state <= s_swhile_start2;
			if ( cnt_registr = X"00" )  then
				next_state <= s_start;
			end if;

		-- END WHILE takt 2
		when swhile_end2 =>
			next_state <= s_start;
			if ( DATA_RDATA = X"00" ) then
				pc_increment <= '1';
			else 
				pc_decrement <= '1';
				cnt_increment <= '1';
				next_state <= swhile_end3;
			end if;

		-- END WHILE takt 3
		when swhile_end3 =>
			next_state <= s_while_miss_back1;
			pc_state <= '1';
			DATA_EN <= '1';

		when s_while_miss_back1 =>
			next_state <= s_while_miss_back2;
			if ( ptr_instr_type = i_while_end ) then
				cnt_increment <= '1';
			elsif ( ptr_instr_type = i_while_start ) then
				cnt_decrement <= '1';
			end if;

		when s_while_miss_back2 =>
			next_state <= s_start;
			if ( cnt_registr = X"00" ) then
				pc_increment <= '1';      
			else 
				next_state <= swhile_end3;
				pc_decrement <= '1';
			end if;
		----------- END WHILE -----------


			when others =>
				next_state <= s_start;

		end case;
	end process;
end behavioral;
 
