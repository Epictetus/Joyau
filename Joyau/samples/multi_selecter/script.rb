initLib
initGfx

def createSelecter(txt, txt2)
  selecter = VerticalMsgSelecter.new
  selecter.resize(100, 272)

  conf = selecter.focusConf 
  conf.borderColor = color(255, 255, 255)
  conf.textColor = color(255, 255, 0)
  conf.titleColor = color(255, 0, 255)
  conf.bgColor = color(0, 0, 255)

  msg = Message.new
  msg.text = txt
  
  msg2 = Message.new
  msg2.text = txt2

  selecter << msg
  selecter << msg2

  return selecter
end

selecter = MultiVerticalMsgSelecter.new
selecter << createSelecter("This is the 1st", "This is the 2nd")
selecter << createSelecter("This is the 3rd", "This is the 4th")

while mayPlay
  Pad.update
  if Pad.pressed? Pad::LEFT
    selecter.select(-1)
  elsif Pad.pressed? Pad::RIGHT
    selecter.select(1)
  end

  actu = selecter.selected
  if Pad.pressed? Pad::UP
    actu.select(-1)
  elsif Pad.pressed? Pad::DOWN
    actu.select(1)
  end

  startDraw
  clearScreen
  selecter.draw
  endDraw
  
  sync
end

stopGfx
stopLib
