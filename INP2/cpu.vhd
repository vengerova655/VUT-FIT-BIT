-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2020 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): xvenge01
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
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WE    : out std_logic;                    -- cteni (0) / zapis (1)
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

---FSM STATES
	type FSMstate is(s_start, s_fetch, s_decode,
		s_val_inc, 
		s_val_dec,
		s_ptr_inc, s_ptr_dec,
		s_get, s_put, s_halt,
		s_while_start, s_while_skip,
		s_while_end);
	signal pState: FSMstate;
	signal nextState: FSMstate;
--/FSM STATES

---RAS
	signal ras_reg: std_logic_vector(11 downto 0);
--/RAS


---DATA POINTER
	signal ptr_reg: std_logic_vector(9 downto 0);
	signal ptr_inc: std_logic;
	signal ptr_dec: std_logic;
--/DATA POINTER 



---PROGRAM COUNTER
	signal pc_reg: std_logic_vector(11 downto 0);
	signal pc_inc: std_logic;
	signal pc_dec: std_logic;
	signal pc_ld: std_logic;
--/PROGRAM COUNTER 

---MULTIPLEXOR
	signal decision: std_logic_vector(1 downto 0);
--/MULTIPLEXOR

begin
-- ------------------------------------------------------------------------------------------------
-- --------------------------------------PC--------------------------------------------------------
pc: process (RESET, CLK, pc_reg, pc_ld, pc_inc, pc_dec)
begin
	if (RESET='1') then
		pc_reg <= (others=>'0');
	elsif (CLK'event) and (CLK='1') then
		if (pc_ld='1') then
			pc_reg <= ras_reg;
		elsif (pc_inc='1') then
			pc_reg<=pc_reg + 1;
		elsif (pc_dec='1') then
			pc_reg<=pc_reg - 1;
		end if;
	end if;
	CODE_ADDR<=pc_reg;
end process;
 
-- ////////////////////////////////////////////////////////////////////////////////////////////////
-- //////////////////////////////////////PC////////////////////////////////////////////////////////

-- ------------------------------------------------------------------------------------------------
-- --------------------------------------PTR-------------------------------------------------------
ptr: process (RESET, CLK, ptr_reg, ptr_inc, ptr_dec)
begin
	if (RESET='1') then
		ptr_reg <= (others => '0');
	elsif (CLK'event) and (CLK='1') then
		if (ptr_inc = '1') then
			ptr_reg <= ptr_reg + 1;
		elsif (ptr_dec='1') then
			ptr_reg<=ptr_reg - 1;
		end if;
	end if;
end process;
 
-- ////////////////////////////////////////////////////////////////////////////////////////////////
-- //////////////////////////////////////PTR///////////////////////////////////////////////////////


-- ------------------------------------------------------------------------------------------------
-- --------------------------------------MUX-------------------------------------------------------
with decision select
	DATA_WDATA <= IN_DATA when "00",
					DATA_RDATA + 1 when "10",
					DATA_RDATA - 1 when "01",
					"00000000" when others;
-- ////////////////////////////////////////////////////////////////////////////////////////////////
-- //////////////////////////////////////MUX///////////////////////////////////////////////////////


 
-- ------------------------------------------------------------------------------------------------
-- --------------------------------------FSM-------------------------------------------------------

 -- ------present state--
	pStateLogic: process (CLK,RESET, EN)
	begin
		if (RESET='1') then
			pState<= s_start;
		elsif (CLK'event) and (CLK='1') then
			if (EN = '1') then
				pState<=nextState;
			end if;
		end if;
	end process;

	
 -- ------next state-----
	nextStateLogic: process(pState, OUT_BUSY, CODE_DATA, DATA_RDATA, IN_VLD, ptr_inc, ptr_dec, pc_reg, ptr_reg, pc_dec, pc_ld, pc_inc)
	begin
		-- inicialization
			ptr_inc<= '0';
			ptr_dec <= '0';
			pc_inc <= '0';
			pc_dec <= '0';
			pc_ld <= '0';
			decision  <= "11";
			
			DATA_ADDR <= ptr_reg;
			DATA_EN <= '1';
			DATA_WE <= '0';
			OUT_WE <= '0';
			IN_REQ <= '0';
			OUT_DATA <= (others => '0');
			
		case pstate is
			when s_start =>
				nextState<=s_fetch;
				
			when s_fetch =>
				CODE_EN<='1';
				nextState<=s_decode;
				
			when s_decode =>
				case CODE_DATA is
					when X"3E" => 
						nextState <= s_ptr_inc;
					when X"3C" =>
						nextState <= s_ptr_dec;
					when X"2B" =>
						nextState <= s_val_inc;
					when X"2D" =>
						nextState <= s_val_dec;
					when X"5B" =>
						DATA_WE <= '0';
						nextState <= s_while_start;
					when X"5D" =>
						DATA_WE <= '0';
						nextState <= s_while_end;
					when X"2E" =>
						nextState <= s_put;
					when X"2C" =>
						nextState <= s_get;
					when X"00" =>
						nextState <= s_halt;
					when others => -- aj komenty
						pc_inc <= '1';
						nextState <= s_fetch;
				end case;
			
			when s_ptr_inc =>
            ptr_inc <= '1';
            pc_inc  <= '1'; 
				nextState <= s_fetch;

        when s_ptr_dec =>
            ptr_dec <= '1';
            pc_inc  <= '1';
				nextState <= s_fetch;
				
			when s_val_inc =>
				DATA_WE <= '1';
				decision <= "10";
				pc_inc <= '1';
				nextState <= s_fetch;
				
			when s_val_dec =>
				DATA_WE <= '1';
				decision <= "01";
				pc_inc <= '1';
				nextState <= s_fetch;
				
			when s_put =>
				nextState <= s_put;
				DATA_WE <= '0';
				if(OUT_BUSY = '0' ) then 
					nextState <= s_fetch;
					OUT_DATA <= DATA_RDATA;
					OUT_WE <= '1';
					pc_inc <= '1';
				end if;
			
			when s_get =>
				nextState <= s_get;
				IN_REQ <= '1';
				if (IN_VLD = '1') then
					DATA_WE <= '1';
					nextState <= s_fetch;
					decision <= "00";
					pc_inc <= '1';
				end if;
				
			
		
				
		when s_while_start =>
			 pc_inc <= '1';
			 ras_reg <= pc_reg;
			 
			 if (DATA_RDATA = "00000000") then
				  nextState <= s_while_skip;
			 else 
				  nextState <= s_fetch;
			 end if;
				
		when s_while_skip =>
				if CODE_DATA = X"5D" then
						pc_inc <= '0';
						nextState <= s_fetch;
				else
					pc_inc <= '1';
					nextState<= s_while_skip;
				end if;
		
		when s_while_end =>
				if DATA_RDATA = "00000000" then
					pc_inc <= '1';
					ras_reg <= "000000000000";
					nextState <= s_fetch;
				else
					pc_ld <= '1';
					nextState<= s_fetch;
				end if;
				
		when s_halt =>
				nextState <= s_halt;
			
			end case;
			
	end process;
-- ////////////////////////////////////////////////////////////////////////////////////////////////
-- //////////////////////////////////////FSM///////////////////////////////////////////////////////

 
end behavioral;
 
