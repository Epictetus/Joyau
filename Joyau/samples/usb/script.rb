initLib
initGfx

initUsb
usbConnect

skip = false

while mayPlay
  hash = usbState

  if !skip
    startDraw
    clearScreen
    if hash["activated"]
      drawText(0, 0, "activated")
    end
    if hash["connected"]
      drawText(0, 10, "cable connected")
    end
    if hash["established"]
      drawText(0, 20, "connection established")
    end
    endDraw
  end

  skip = sync
end

usbDisconnect
stopUsb

stopGfx
stopLib
