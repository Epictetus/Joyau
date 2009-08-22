initLib
initGfx

bg = GameMap.new("bg_tile.png")
obstacles = GameMap.new("obs_tile.png")

bg.pos = Point.new(0, 0)
obstacles.pos = Point.new(0, 0)

bg.setTileSize(32, 32)
obstacles.setTileSize(32, 32)

# let's draw a really ugly background !

for i in 0..20
  bg.addElem(0, 0, 0, i * 32, 0)
  bg.addElem Tile.new(0, 32, 0, i * 32, 32)
end

obstacles << Tile.new(0, 0, 0, 0, 0)
obstacles.addElem(0, 0, 0, 5 * 32, 5 * 32)

angel = Sprite.new("angel.png")
angel.setAnim(3, 4)

angel.direction = $directions["DOWN"]
angel.setPos(240 - angel.w / 2, 136 - angel.h / 2)

skip = false

while mayPlay
  readKeys

  if $keys["up"]
    bg.move(0, 2)
    obstacles.move(0, 2)

    angel.direction = $directions["UP"]
  end

  if $keys["down"]
    bg.move(0, -2)
    obstacles.move(0, -2)
    
    angel.direction = $directions["DOWN"]
  end
  
  if $keys["left"]
    bg.move(2, 0)
    obstacles.move(2, 0)

    angel.direction = $directions["LEFT"]
  end

  if $keys["right"]
    bg.move(-2, 0)
    obstacles.move(-2, 0)

    angel.direction = $directions["RIGHT"]
  end

  if obstacles.collide(angel)
    bg.cancelMove
    obstacles.cancelMove
  end

  if !skip
    startDraw

    clearScreen # The map is already ugly...

    bg.draw
    obstacles.draw
    angel.draw

    endDraw
  end
  
  skip = sync
end

stopGfx
stopLib
