Joyau.initLib
Joyau.initGfx

angel = Joyau::Sprite.new("angel.png")
angel.setAnim(3, 4)

angel.direction = Joyau::Sprite::DOWN

bg = Joyau::Sprite.new("bg.png")

while Joyau.mayPlay
  Joyau.readKeys

  if $keys["up"]
    angel.move(0, -2)
    angel.direction = Joyau::Sprite::UP
  end
  if $keys["down"]
    angel.move(0, 2)
    angel.setDirection(Joyau::Sprite::DOWN) # this works too
  end
  if $keys["left"]
    angel.move(-2, 0)
    angel.dir = Joyau::Sprite::LEFT # as well as this
  end
  if $keys["right"]
    angel.move(2, 0)
    angel.direction = Joyau::Sprite::RIGHT
  end

  Joyau.startDraw
  bg.draw
  angel.draw
  Joyau.endDraw

  Joyau.sync
end

Joyau.stopGfx
Joyau.stopLib
