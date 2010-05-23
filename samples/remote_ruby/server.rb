#!/usr/bin/ruby

# This script must be run on your computer,
# not on your PSP.
#
# Once the homebrew is started, you can run command from your computer.
# 

require 'drb/drb'
require 'readline'

class CommandGiver
  def get_instruction
    code = ""

    loop do
      line = Readline.readline(code == "" ? '=> ' : '', true)
      if line =~ /^\s*$/ or Readline::HISTORY.to_a[-2] == line
        Readline::HISTORY.pop
      end
      
      code << (line + "\n")
      break if code[-1] == "\n" and code[-2] == "\n"
    end

    return code
  end

  def send_answer(result)
    puts "Result: #{result}"
  end

  def send_error(error)
    puts error
  end
end

SERVER_URI = "druby://#{ARGV[0] || "0.0.0.0" }:8787"
puts "Server started at #{SERVER_URI}"

DRb.start_service(SERVER_URI, CommandGiver.new)
DRb.thread.join
