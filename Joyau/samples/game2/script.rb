initLib
initGfx

speed = 3

scroll = Scrolling.new
scroll.setSprite("bg.png")
scroll.setPos(0, 0)
scroll.setSpeed(speed)
scroll.setDir($directions["LEFT"])

obstacles = []
for i in 0..4 # having 5 obstacles is great
  obstacles[i] = Sprite.new
  obstacles[i].setPicture("obstacle.png")
  obstacles[i].setPos(rand(480), rand(272))
end

hero = Sprite.new
hero.setPicture("angel.png")
hero.setAnim(3, 4)
hero.setDirection($directions["RIGHT"])
hero.setPos(480 - hero.getW, 272 - hero.getH)

score = 0
life = 100
startTimestamp = timestamp

no_color = Hash.new
no_color["r"] = 0
no_color["g"] = 0
no_color["b"] = 0
no_color["a"] = 0

setTextBackground(no_color)

while mayPlay and life > 0
  # We'll let the hero play
  readKeys
  if $keys["down"]
    if hero.getY + 1 < 272 - hero.getW
      hero.move(0, 1)
    end
  elsif $keys["up"]
    if hero.getY - 1 > 0
      hero.move(0, -1)
    end
  end

  if $keys["right"]
    hero.move(speed, 0)
  else
    hero.move(-speed * 2, 0)
  end

  # He'll lose life if he isn't in the map :
  if hero.getX >= 480
    life -= 1
    hero.move(-10, 0)
  elsif hero.getX <= 0
    life -= 1
    hero.move(10, 0)
  end

  # Then, it's the obstacle turn :
  for i in obstacles
    i.move(-speed, 0)
    if i.collide(hero)
      life -= 1
      hero.move(-i.getW, 0)
    end
    if i.getX < 0 - i.getW
      i.setPos(480, rand(272))
    end
  end

  scroll.play
  score = timestamp - startTimestamp

  startDraw
  scroll.draw
  for i in obstacles
    i.draw
  end
  hero.draw

  drawText(0, 0, "Score : " + score.to_s)
  drawText(0, 10, "Life : " + life.to_s)

  endDraw

  sync
end

stopLib
stopGfx
