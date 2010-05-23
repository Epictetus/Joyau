require 'open-uri'

require 'joyau/init'
require 'joyau/color'

def show_message(msg)
  Joyau.draw do
    Joyau.clearScreen
    Joyau.drawScripted(0, 0, msg)
  end
end

Joyau.init(:wlan, :gfx)

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

times = 0
begin
  Joyau::Wlan.connect(selecter.index + 1, 60)
rescue Joyau::TimeoutError
  times += 1
  if times < 3
    show_message "Failled to connect. Trying again."
    retry
  else
    show_message "Failled to connect. Exiting"
    Joyau.exitGame
  end
end

url = Joyau::OSK.run(Joyau::OSK::URL, Joyau.language, 
                     "http://www.google.com", "Enter a valid URL",
                     256) do
  Joyau.clearScreen
end

target = Joyau::OSK.run(Joyau::OSK::URL, Joyau.language, 
                     "index.html", "Enter the local filename.",
                     256) do
  Joyau.clearScreen
end

open(url) do |stream|
  open(target, 'w') do |file|
    file.write stream.read
  end
end

Joyau::Pad.update
show_message "File saved. Presse X to exit." until 
  Joyau::Pad.pressed? Joyau::Pad::CROSS

Joyau.stop
