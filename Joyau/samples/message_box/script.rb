# -*- coding: utf-8 -*-

def drawMessage(hero)
  message = Message.new
  message.setPos(0, 0)
  message.setTitlePos(50, 5)
  message.resize(480, 40)
  message.setTitle("press X")

  red = Hash.new
  red["a"] = 255
  red["r"] = 255
  red["b"] = 0
  red["a"] = 0
  message.setTitleColor(red)

  message.setText("Alpha : " + hero.getAlpha.to_s)
  
  sprite = Sprite.new
  sprite.setPicture("pic.png")

  message.setImage(sprite)

  wouldContinue = true
  while wouldContinue and mayPlay
    readKeys
    if $keys["cross"]
      wouldContinue = false
    end

    startDraw
    message.draw
    endDraw

    sync
  end

end

initLib
initGfx

hero = Sprite.new
hero.setPicture("pic.png")
hero.setPos(240 - hero.getW / 2, 136 - hero.getH / 2)

bg = Sprite.new
bg.setPicture("bg.png")

while mayPlay
  readKeys
  if $keys["up"]
    hero.move(0, -2)
  end
  if $keys["down"]
    hero.move(0, 2)
  end
  if $keys["left"]
    hero.move(2, 0)
  end
  if $keys["right"]
    hero.move(-2, 0)
  end
  if $keys["cross"]
    if hero.getAlpha - 2 > 30
      hero.setAlpha(hero.getAlpha - 2)
    end
  end
  if $keys["triangle"]
    if hero.getAlpha + 2 < 256
      hero.setAlpha(hero.getAlpha + 2)
    end
  end
  if $keys["square"]
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
