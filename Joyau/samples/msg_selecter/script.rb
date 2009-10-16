require 'joyau/old'

initLib
initGfx

selecter = VerticalMsgSelecter.new
selecter.resize(100, 272)

conf = selecter.focusConf 
conf.borderColor = color(255, 255, 255)
conf.textColor = color(255, 255, 0)
conf.titleColor = color(255, 0, 255)
conf.bgColor = color(0, 0, 255)

msg = Message.new
msg.text = "This is the first message."

msg2 = Message.new
msg2.text = "This is the second message."

selecter << msg
selecter << msg2

while mayPlay
  Pad.update
  if Pad.pressed? Pad::UP
    selecter.select(VerticalMsgSelecter::PREV)
  elsif Pad.pressed? Pad::DOWN
    selecter.select(VerticalMsgSelecter::NEXT)
  end

  startDraw
  clearScreen
  selecter.draw
  endDraw
  
  sync
end

stopGfx
stopLib
