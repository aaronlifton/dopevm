# inputs lines of space-seperated integers,
# turns it into c code,
# appends it to programs.h
# then runs 'make clean && make'

require 'IO'

puts "Enter an instruction as defined in the README on each line.\nPress enter to go to the next line.\nType END to finish."

$/ = "END"
user_input = STDIN.gets

IO.read("programs.h")

puts user_input
