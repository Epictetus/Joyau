require 'joyau/old'

initLib
initGfx

keys = [ "select", "start", "up", "down", "right", "left", "L", "R", "cross",
         "triangle", "square", "hold" ]

while mayPlay
  Pad.update

  heldMsg = ""
  releasedMsg = ""
  pressedMsg = ""
  
  keys.each { |key|
    if Pad.held? key
      heldMsg += key + " is held\n"
    end
    if Pad.pressed? key
      pressedMsg += key + " is pressed\n"
    end
    if Pad.released? key
      releasedMsg += key + " is released\n"
    end
  }

  startDraw
  clearScreen
  drawScripted(0, 0, heldMsg)
  drawScripted(100, 0, pressedMsg)
  drawScripted(200, 0, releasedMsg)
  endDraw
  
  sync
end

stopGfx
stopLib
