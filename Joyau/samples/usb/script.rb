initLib
initGfx

Usb.init
Usb.connect

skip = false

while mayPlay
  if !skip
    startDraw
    clearScreen
    if Usb.activated?
      drawText(0, 0, "activated")
    end
    if Usb.connected?
      drawText(0, 10, "cable connected")
    end
    if Usb.established?
      drawText(0, 20, "connection established")
    end
    endDraw
  end

  skip = sync
end

Usb.disconnect
Usb.stop

stopGfx
stopLib
