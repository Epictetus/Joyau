require 'joyau/old'

def drawMessage(hero)
  message = Message.new
  message.pos = Point.new(0, 0)
  message.titlePos = Point.new(477 - getLength("press X"), 10)

  message.resize(480, 40)
  message.title = "press X"

  message.titleColor = color(255, 0, 0)
  message.text = "Alpha : " + hero.alpha.to_s
  
  message.image = Sprite.new("pic.png")

  while mayPlay
    Pad.update
    if Pad.held? Pad::CROSS
      break
    end

    startDraw
    message.draw
    endDraw

    sync
  end

end

initLib
initGfx

hero = Sprite.new("pic.png")
hero.setPos(240 - hero.w / 2, 136 - hero.h / 2)

bg = Sprite.new("bg.png")

while mayPlay
  Pad.update
  
  if Pad.held? Pad::UP
    hero.move(0, -2)
  end
  if Pad.held? Pad::DOWN
    hero.move(0, 2)
  end
  if Pad.held? Pad::LEFT
    hero.move(-2, 0)
  end
  if Pad.held? Pad::RIGHT
    hero.move(2, 0)
  end

  if Pad.held? Pad::CROSS
    if hero.alpha - 2 > 30
      hero.alpha -= 2
    end
  end
  if Pad.held? Pad::TRIANGLE
    if hero.alpha + 2 < 256
      hero.alpha += 2
    end
  end
  if Pad.held? Pad::SQUARE
    drawMessage(hero)
  end

  startDraw
  bg.draw
  hero.draw
  endDraw

  sync
end

stopGfx
stopLib
