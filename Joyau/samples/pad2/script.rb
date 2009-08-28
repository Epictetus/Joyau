initLib
initGfx

keys = [ Pad::SELECT, Pad::START, Pad::L, Pad::R, Pad::CROSS, Pad::SQUARE,
         Pad::TRIANGLE, Pad::CIRCLE, Pad::UP, Pad::DOWN, Pad::LEFT, Pad::RIGHT,
         Pad::HOLD ]

while mayPlay
  Pad.update

  heldNbr = keys.inject(0) { |sum, i| Pad.held?(i) ? sum + 1 : sum }.to_s
  releasedNbr = keys.inject(0) { |sum, i| Pad.released?(i) ? sum + 1 : sum }.to_s
  pressedNbr = keys.inject(0) { |sum, i| Pad.pressed?(i) ? sum + 1 : sum }.to_s

  startDraw
  clearScreen
  drawScripted(0, 0, heldNbr + " keys are held.")
  drawScripted(0, 10, pressedNbr + " keys are pressed.")
  drawScripted(0, 20, releasedNbr + " keys are released.")
  endDraw
  
  sync
end

stopGfx
stopLib
