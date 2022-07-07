library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

-- rozhrani Vigenerovy sifry
entity vigenere is
   port(
         CLK : in std_logic;
         RST : in std_logic;
         DATA : in std_logic_vector(7 downto 0);
         KEY : in std_logic_vector(7 downto 0);

         CODE : out std_logic_vector(7 downto 0)
    );
end vigenere;

-- vypracovala VERONIKA VENGEROVA xvenge01
-- V souboru fpga/sim/tb.vhd naleznete testbench, do ktereho si doplnte
-- znaky vaseho loginu (velkymi pismeny) a znaky klice dle vaseho prijmeni.
-- DONE

architecture behavioral of vigenere is
	signal shift: std_logic_vector(7 downto 0);
	signal plus: std_logic_vector(7 downto 0);
	signal minus: std_logic_vector(7 downto 0);
	
	signal fsmMealy: std_logic_vector(1 downto 0);


	type FSMstate is (add,sub);
	signal pstate: FSMstate;
	signal nextState: FSMstate;
    -- Sem doplnte definice vnitrnich signalu, prip. typu, pro vase reseni,
    -- jejich nazvy doplnte tez pod nadpis Vigenere Inner Signals v souboru
    -- fpga/sim/isim.tcl. Nezasahujte do souboru, ktere nejsou explicitne
    -- v zadani urceny k modifikaci.


begin


-- vypocet velikosti posuvu
	procesZmeny: process (KEY)
	begin
		-- A(ASCII 65) -> posun o 1 -> 65-64
		-- B (ASCII 66) -> posun o 2 -> 66-64 ....
		shift <= KEY - 64;
	end process;	
	
-- + s korekci
	plusProces: process (DATA,shift)
	begin
		plus <= DATA + shift;
		--ak mimo abecedy tak opravit a od zaciatku menit znaky
		if ((DATA + shift) >= 91) then
			plus <= (DATA + shift) - 26;
		end if;
	end process;	

-- - s korekci
	minusProces: process(DATA,shift)
	begin
		minus <= DATA - shift;
		--ak mimo abecedy tak opravit a od konca opat menit znaky
		if ((DATA-shift) <= 64) then
			minus <= DATA - shift + 26;
		end if;
	end process;

-- FSM Mealy
	--present state
	pstateLogic: process(CLK,RST)
	begin
		if RST = '1' then
			pstate <= add;
		elsif (CLK'event) and (CLK='1') then
			pstate <= nextState;
		end if;
	end process;

	--next state
	nextstateLogic: process (pstate,RST,DATA)
	begin
		nextState <= pstate;
		-- defaultne na hashtag
		fsmMealy <= "00";
		case pstate is
			when add =>
				nextState <= sub;
				-- menit ak nejde o cislo/reset
				if not (DATA>47 and DATA<58) and not (RST='1') then
					fsmMealy <="01";
				end if;
			when sub =>
				nextState <= add;
				-- menit ak nejde o cislo/reset
				if (not (DATA>47 and DATA<58)) and (not (RST='1')) then
					fsmMealy <= "10";
				end if;
			when others => null;
		end case;	
	end process;

-- multiplexor
	with fsmMealy select
		CODE <= plus when "01",
			minus when "10",
			-- hashtag = 00100011, na output ak nejde o velke pismeno a pri resete
			"00100011" when others;	


    -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.


end behavioral;
