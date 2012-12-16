# inputs lines of space-seperated integers,
# turns it into c code,
# appends it to programs.h
# then runs 'make clean && make'
# NOTE:
# not yet working, so write your programs as opcode
# arrays in programs.h, then use writer ob to
# compile them to binary programs that can run
# on dopevm
require 'rubygems'
require './c_parser'

#puts "Enter an instruction as defined in the README on each line.\nPress enter to go to the next line.\nType END to finish."
#$/ = "END"
#user_input = STDIN.gets

filename = "programs.h"

parser = CParser.new
ccode = IO.popen(filename){|f| f.read}

puts parser.parse(ccode)
[{:function=>
   [{:type=>{:keyword=>"int"}},
    {:type=>{:keyword=>"int"}},
    {:type=>{:keyword=>"char"}},
    {:array=>{:size=>nil}},
    {:body=>
      [{:declarations=>
         [{:type=>{:keyword=>"int"}},
          {:array=>{:size=>{:constant=>"1"}}}]}]}]}]