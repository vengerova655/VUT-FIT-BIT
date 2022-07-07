-- fsm.vhd: Finite State Machine
-- Author(s): xvenge01
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
   type t_state is (INCORRECT, TEST_BEG,	PRINT_INCORRECT,  PRINT_CORRECT, FINISH,
	TEST_9, TEST_93, TEST_938, TEST_9381,TEST_93811, TEST_938118,
	TEST_9381185, TEST_93811856,  TEST_938118560,
	TEST_1,  TEST_15,  TEST_156, TEST_1563,  TEST_15635,
	TEST_156353,  TEST_1563530,  TEST_15635309, TEST_156353093, TEST_1563530933);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_BEG;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_BEG =>
      next_state <= TEST_BEG;
		if (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
      elsif (KEY(9) = '1') then
         next_state <= TEST_9;
		elsif (KEY(1) = '1') then
			next_state <= TEST_1;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when  TEST_1 =>
      next_state <= TEST_1;
		if (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
      elsif (KEY(5) = '1') then
         next_state <= TEST_15;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
   -- - - - - - - - - -  - - - - - - - - - - - - -
	 when  TEST_15 =>
      next_state <= TEST_15;
		if (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
      elsif (KEY(6) = '1') then
         next_state <= TEST_156;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
   -- - - - - - - - - -  - - - - - - - - - - - - -
    when  TEST_156 =>
      next_state <= TEST_156;
      if (KEY(3) = '1') then
         next_state <= TEST_1563;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - -  - - - - - - - - - - - - -
    when  TEST_1563 =>
      next_state <= TEST_1563;
		if (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
      elsif (KEY(5) = '1') then
         next_state <= TEST_15635;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - -  - - - - - - - - - - - - -
    when  TEST_15635 =>
      next_state <= TEST_15635;
      if (KEY(3) = '1') then
         next_state <= TEST_156353;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - -  - - - - - - - - - - - - -
    when  TEST_156353 =>
      next_state <= TEST_156353;
      if (KEY(0) = '1') then
         next_state <= TEST_1563530;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - -  - - - - - - - - - - - - -
    when  TEST_1563530 =>
      next_state <= TEST_1563530;
      if (KEY(9) = '1') then
         next_state <= TEST_15635309;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - -  - - - - - - - - - - - - -
    when  TEST_15635309 =>
      next_state <= TEST_15635309;
      if (KEY(3) = '1') then
         next_state <= TEST_156353093;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - -  - - - - - - - - - - - - -
    when  TEST_156353093 =>
      next_state <= TEST_156353093;
      if (KEY(3) = '1') then
         next_state <= TEST_1563530933;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - - - - - - - - - - - - - - -
    when  TEST_1563530933 =>
      next_state <= TEST_1563530933;
		if (KEY(15) = '1') then
			next_state <= PRINT_CORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_9 =>
      next_state <= TEST_9;
      if (KEY(3) = '1') then
         next_state <= TEST_93;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	 -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_93 =>
      next_state <= TEST_93;
      if (KEY(8) = '1') then
         next_state <= TEST_938;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_938 =>
      next_state <= TEST_938;
      if (KEY(1) = '1') then
         next_state <= TEST_9381;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_9381 =>
      next_state <= TEST_9381;
      if (KEY(1) = '1') then
         next_state <= TEST_93811;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_93811 =>
      next_state <= TEST_93811;
      if (KEY(8) = '1') then
         next_state <= TEST_938118;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_938118 =>
      next_state <= TEST_938118;
      if (KEY(5) = '1') then
         next_state <= TEST_9381185;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_9381185 =>
      next_state <= TEST_9381185;
      if (KEY(6) = '1') then
         next_state <= TEST_93811856;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_93811856 =>
      next_state <= TEST_93811856;
      if (KEY(0) = '1') then
         next_state <= TEST_938118560;
		elsif (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_938118560 =>
      next_state <= TEST_938118560;
		if (KEY(15) = '1') then
			next_state <= PRINT_CORRECT;
		elsif  (KEY(14 downto 0) /= "000000000000000") then
			next_state <= INCORRECT;
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
   when INCORRECT =>
		if (KEY(15) = '1') then
			next_state <= PRINT_INCORRECT;
		else
			 next_state <= INCORRECT;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_INCORRECT =>
      next_state <= PRINT_INCORRECT;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_CORRECT =>
      next_state <= PRINT_CORRECT;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST_BEG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST_BEG;
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
   when INCORRECT| TEST_BEG| TEST_9| TEST_93| TEST_938| TEST_9381| TEST_93811| TEST_938118| TEST_9381185| TEST_93811856| TEST_938118560 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
	-- - - - - - - - - - - - - - - - - - - - - - -
	 when TEST_1| TEST_15| TEST_156| TEST_1563| TEST_15635| TEST_156353| TEST_1563530| TEST_15635309| TEST_156353093|  TEST_1563530933 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_INCORRECT =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	-- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_CORRECT =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
		FSM_MX_MEM     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

