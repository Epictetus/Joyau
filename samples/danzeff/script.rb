require 'joyau/danzeff'

Joyau.initLib
Joyau.initGfx

keyboard = Joyau::Danzeff.new
keyboard.w, keyboard.h = 200, 200

Joyau.setTextBackground(Joyau::Color::NONE)

while Joyau.mayPlay
  Joyau::Pad.update

  Joyau.startDraw
  Joyau.clearScreen
  keyboard.draw
  Joyau.drawScripted(480 - Joyau.getLength(keyboard.str), 260, keyboard.str)
  Joyau.endDraw

  Joyau.sync
end

Joyau.stopGfx
Joyau.stopLib
