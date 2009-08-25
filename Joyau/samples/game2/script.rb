initLib
initGfx

speed = 3

scroll = Scrolling.new("bg.png")
scroll.setSprite("bg.png")
scroll.pos = Point.new(0, 0)
scroll.speed = speed
scroll.setDir($directions["LEFT"])

obstacles = []
5.times {
  obs = Sprite.new("obstacle.png")
  obs.pos = Point.new(rand(480), rand(272))
  obstacles << obs
}

hero = Sprite.new("angel.png")
hero.setAnim(3, 4)
hero.direction = $directions["RIGHT"]
hero.pos = Point.new(480 - hero.w, 272 - hero.h)
 
score = 0
life = 100

timer = Timer.new
 
while mayPlay and life > 0
  # We'll let the hero play
  readKeys
  if $keys["down"]
    if hero.y + 1 < 272 - hero.w
      hero.move(0, 1)
    end
  elsif $keys["up"]
    if hero.y - 1 > 0
      hero.move(0, -1)
    end
  end
 
  if $keys["right"]
    hero.move(speed, 0)
  else
    hero.move(-speed * 2, 0)
  end
 
  # He'll lose life if he isn't in the map :
  if hero.x >= 480
    life -= 1
    hero.move(-10, 0)
  elsif hero.x <= 0
    life -= 1
    hero.move(10, 0)
  end
 
  # Then, it's the obstacle turn :
  obstacles.each { |obs|
    obs.move(-speed, 0)
    if obs.collide(hero)
      life -= 1
      hero.move(-obs.w, 0)
    end
    if obs.x < 0 - obs.w
      obs.pos = Point.new(480, rand(272))
    end
  }
 
  scroll.play
   
  startDraw
  scroll.draw
  obstacles.each { |obs| obs.draw }
  hero.draw
 
  drawScripted(0, 0, "Score : " + timer.time.to_s)
  drawScripted(0, 10, "Life : " + life.to_s)
 
  endDraw
 
  sync
end
 
stopLib
stopGfx
