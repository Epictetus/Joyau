require 'joyau/enumerable'

Joyau.initLib
Joyau.initGfx

sprite = Joyau::Sprite.new("angel.png")
sprite.setAnim(3, 4)
sprite.autoDir = true

until Joyau::Pad.pressed? Joyau::Pad::CROSS
  Joyau::Pad.update
  Joyau.draw do
    sprite.draw
  end
end

buf = sprite.to_buf! # /!\ We are going to modify the buffer
                     # that Joyau has stored in its database.
                     # It is not recommanded in real, complex, programs.
buf.lock do          # And we should really lock the buffer in such cases.
  buf.each_with_pos do |x, y, col|
    gray = (0.299 * col.r + 0.587 * col.g + 0.114 * col.b).to_i
    buf[x, y] = Joyau::Color.new(gray, gray, gray, col.a)
  end
end

while Joyau.mayPlay
  Joyau::Pad.update

  sprite.move(2, 0)  if Joyau::Pad.held? Joyau::Pad::RIGHT
  sprite.move(-2, 0) if Joyau::Pad.held? Joyau::Pad::LEFT
  sprite.move(0, -2) if Joyau::Pad.held? Joyau::Pad::UP
  sprite.move(0, 2)  if Joyau::Pad.held? Joyau::Pad::DOWN
  
  Joyau.draw do
    Joyau.clearScreen
    sprite.draw
  end
end

Joyau.stopGfx
Joyau.stopLib
