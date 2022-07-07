proc isim_script {} {

   add_divider "Signals of the Vigenere Interface"
   add_wave_label "" "CLK" /testbench/clk
   add_wave_label "" "RST" /testbench/rst
   add_wave_label "-radix ascii" "DATA" /testbench/tb_data
   add_wave_label "-radix ascii" "KEY" /testbench/tb_key
   add_wave_label "-radix ascii" "CODE" /testbench/tb_code

   add_divider "Vigenere Inner Signals"
   
   
   add_wave_label "-radix dec"  "shift" /testbench/uut/shift
   add_wave_label "-radix ascii" "plus" /testbench/uut/plus
   add_wave_label "-radix ascii" "minus" /testbench/uut/minus
   add_wave_label "" "fsmMealy" /testbench/uut/fsmMealy


   
   add_wave_label "" "pstate" /testbench/uut/pstate
   add_wave_label "" "nextState" /testbench/uut/nextState

   # sem doplnte vase vnitrni signaly. chcete-li v diagramu zobrazit desitkove
   # cislo, vlozte do prvnich uvozovek: -radix dec

   run 8 ns
}
