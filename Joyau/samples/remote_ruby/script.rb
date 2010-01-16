require 'joyau/init'
require 'joyau/color'
require 'joyau/drb'

Joyau.init(:gfx, :wlan)

def show_message(msg)
  Joyau.draw do
    Joyau.clearScreen
    Joyau.drawScripted(0, 0, msg)
  end
end

unless Joyau::Wlan.button_enabled?
  show_message "Please press the WLAN buton."
  sleep 0.1 until Joyau::Wlan.button_enabled?
end

if Joyau::Wlan.configs.size == 0
  show_message "You need an acces point to run this program."
  Joyau.exitGame
end

selecter = Joyau::VerticalMsgSelecter.new

sel_conf = selecter.focusConf
sel_conf.borderColor = Joyau::Color::NONE
sel_conf.textColor   = Joyau::Color::RED
sel_conf.titleColor  = Joyau::Color::NONE
sel_conf.bgColor     = Joyau::Color::NONE

sel_conf = selecter.conf
sel_conf.borderColor = Joyau::Color::NONE
sel_conf.textColor   = Joyau::Color::WHITE
sel_conf.titleColor  = Joyau::Color::NONE
sel_conf.bgColor     = Joyau::Color::NONE

selecter.move(0, 15)

Joyau::Wlan.configs.map { |conf| conf[1] }.each do |name| 
  msg = Joyau::Message.new
  msg.text = name
  selecter << msg
end

selecter.resize(100, selecter.size * 10)

while Joyau.mayPlay
  Joyau::Pad.update

  if Joyau::Pad.pressed? Joyau::Pad::UP
    selecter.select -1
  elsif Joyau::Pad.pressed? Joyau::Pad::DOWN
    selecter.select 1
  end

  Joyau.draw do
    Joyau.clearScreen
    Joyau.drawScripted(0, 0, "Select an access point:")
    selecter.draw
  end

  break if Joyau::Pad.pressed? Joyau::Pad::CROSS
end

show_message "Connecting to access point..."

tested = 0
begin
  Joyau::Wlan.connect(selecter.index + 1, 60)
  show_message "Connecting to access point... done !"
rescue Joyau::TimeoutError
  tested += 1
  if tested < 3
    show_message "Connecting to access point... timeouted !\n Trying again."
    retry
  else
    show_message "Connecting to access point... failled !"
    Joyau.exitGame
  end
end

DRb.start_service

SERVER_URI = "druby://192.168.1.4:8787"

show_message "Press X if the server (#{SERVER_URI}) is running."

Joyau::Pad.update
until Joyau::Pad.pressed? Joyau::Pad::CROSS
  Joyau::Pad.update
  sleep 0.1
end

show_message "Connecting to #{SERVER_URI}..."
command_giver = DRbObject.new_with_uri(SERVER_URI)
show_message "Connected to the server !"

while Joyau.mayPlay
  line = command_giver.get_instruction
  
  begin
    command_giver.send_answer eval(line).inspect
  rescue
    command_giver.send_error "#{$!.class}: #{$@[0]}: #{$!.message}"
  end
end

Joyau::Wlan.disconnect
Joyau.stop
