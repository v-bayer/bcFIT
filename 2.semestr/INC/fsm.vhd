-- fsm.vhd: Finite State Machine
-- Author(s): Vaclav Bayer | xbayer05
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (STEP, STEP1, STEP2, STEP3, STEP4, STEP5, STEP6, STEP7, STEP8, STEP9, STEP10, STEP12, STEP13, STEP14, STEP15, STEP16, STEP17, WRONG, FINISH, PRINT_MESSAGE_OK, PRINT_MESSAGE_WRONG);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= STEP1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP
   when STEP =>
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 1 (3)  // obe
   when STEP1 =>
      next_state <= STEP1;
      if (KEY(3) = '1') then
         next_state <= STEP2;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 2 (7) // obe 
   when STEP2 =>
      next_state <= STEP2;
      if (KEY(7) = '1') then
         next_state <= STEP3;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 3 (8) // obe
   when STEP3 =>
      next_state <= STEP3;
      if (KEY(8) = '1') then
         next_state <= STEP4;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 4 (9) // obe
   when STEP4 =>
      next_state <= STEP4;
      if (KEY(9) = '1') then
         next_state <= STEP5;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;  
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 5 (7,0) // obe
   when STEP5 =>
      next_state <= STEP5;
      if (KEY(7) = '1') then
         next_state <= STEP6;
      elsif (KEY(0) = '1') then
         next_state <= STEP12;	
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 6 (6) //prvni
   when STEP6 =>
      next_state <= STEP6;
      if (KEY(6) = '1') then
         next_state <= STEP7;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 7 (6) // prvni
   when STEP7 =>
      next_state <= STEP7;
      if (KEY(6) = '1') then
         next_state <= STEP8;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;		 	 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 8 (0) //prvni
   when STEP8 =>
      next_state <= STEP8;
      if (KEY(0) = '1') then
         next_state <= STEP9;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 9 (8) //prvni
   when STEP9 =>
      next_state <= STEP9;
      if (KEY(8) = '1') then
         next_state <= STEP10;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 10 (8) //prvni
   when STEP10 =>
      next_state <= STEP10;
      if (KEY(8) = '1') then
         next_state <= STEP17;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	       next_state <= WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - - 

   --------------------- DRUHE HESLO -----------------------------

   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 12 (2) //druhe
   when STEP12 =>
      next_state <= STEP12;
      if (KEY(2) = '1') then
         next_state <= STEP13;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	

   
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 13 (1) //druhe
   when STEP13 =>
      next_state <= STEP13;
      if (KEY(1) = '1') then
         next_state <= STEP14;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	    elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	  

   -- - - - - - - - - - - - - - - - - - - - - - -  ////  STEP 14 (4) //druhe
   when STEP14 =>
      next_state <= STEP14;
      if (KEY(4) = '1') then
         next_state <= STEP15;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
      elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;	 
      end if;	  

   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 15 (5) //druhe
   when STEP15 =>
      next_state <= STEP15;
      if (KEY(5) = '1') then
         next_state <= STEP16;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 16 (0) //druhe
   when STEP16 =>
      next_state <= STEP16;
      if (KEY(0) = '1') then
         next_state <= STEP17;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;		 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - - ////  STEP 17 (#)
   when STEP17 =>
      next_state <= STEP17;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_OK;	 
	  elsif (KEY(15 downto 0) /= "0000000000000000") then
	     next_state <= WRONG;
      end if;	  
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRONG =>
      next_state <= WRONG;
      if (KEY(15) = '1') then
         next_state <= PRINT_MESSAGE_WRONG;	 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      next_state <= PRINT_MESSAGE_OK;
      if (CNT_OF = '1') then
		next_state <= FINISH;
	  end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_WRONG =>
      next_state <= PRINT_MESSAGE_WRONG;
      if (CNT_OF = '1') then
		next_state <= FINISH;
	  end if;
   -- - - - - - - - - - - - - - - - - - - - - - -   
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= STEP1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= STEP;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is

   -- - - - - - - - - - - - - - - - - - - - - - -   
   when STEP =>
   -- - - - - - - - - - - - - - - - - - - - - - -   
   when WRONG =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;		  
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_WRONG =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_MESSAGE_OK =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	  FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -   
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -	  
   end case;
end process output_logic;

end architecture behavioral;