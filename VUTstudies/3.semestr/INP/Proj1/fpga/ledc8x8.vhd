library IEEE;
	use IEEE.std_logic_1164.all;
	use IEEE.std_logic_unsigned.all;
	use IEEE.std_logic_arith.all;

	entity ledc8x8 is
		port (
   			SMCLK, RESET: in std_logic;
   			ROW, LED: out std_logic_vector(0 to 7)
		);
	end ledc8x8;

	architecture behavioral of ledc8x8 is
    	signal ce: std_logic;
    	signal row_sig: std_logic_vector(7 downto 0) := "00000001";
    	signal col_sig: std_logic_vector(7 downto 0);
    	signal smclk_sig: std_logic_vector(21 downto 0) := "0000000000000000000000";
		begin
     		CTRLER :process (SMCLK,RESET)
				begin
     			if (RESET = '1') then
						smclk_sig <= "0000000000000000000000";
					elsif (SMCLK'event and SMCLK = '1') then
						smclk_sig <= smclk_sig + 1;
						if (smclk_sig (7 downto 0) = "11111111") then
							ce <= '1';
						else
							ce <= '0';
	    			end if;
					end if;
   			end process;

   			ROTATER :process (ce,RESET)
				begin
					if (RESET = '1') then
						row_sig <= "00000001";
					elsif (ce'event and ce = '1') then
						row_sig <= row_sig(0) & row_sig(7 downto 1);
					end if;
				end process;

				COLSWITCHER :process (row_sig)
				begin
					case row_sig is
						when "10000000" => col_sig <= "00001111";
						when "01000000" => col_sig <= "01110111";
						when "00100000" => col_sig <= "00001111";
						when "00010000" => col_sig <= "01100110";
						when "00001000" => col_sig <= "00001110";
						when "00000100" => col_sig <= "11101110";
						when "00000010" => col_sig <= "11110101";
						when "00000001" => col_sig <= "11111011";
						when others => null;
					end case;
				end process;

				DISPLAYER :process (col_sig)
				begin
					ROW <= row_sig;
					LED <= col_sig;
				end process;
	end behavioral;







