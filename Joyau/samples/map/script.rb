initLib
initGfx

bg = GameMap.new
obstacles = GameMap.new

bg.setPos(0, 0)
obstacles.setPos(0, 0)

bg.addTileset("bg_tile.png")
obstacles.addTileset("obs_tile.png")

bg.setTileSize(32, 32)
obstacles.setTileSize(32, 32)

# let's draw a really ugly background !

for i in 0..20
  bg.addElem(0, 0, 0, i * 32, 0)
  bg.addElem(0, 32, 0, i * 32, 32)
end

obstacles.addElem(0, 0, 0, 0, 0)
obstacles.addElem(0, 0, 0, 5 * 32, 5 * 32)

angel = Sprite.new
angel.setPicture("angel.png")
angel.setAnim(3, 4)

angel.setDirection($directions["DOWN"])
angel.setPos(240 - angel.getW / 2, 136 - angel.getH / 2)

while mayPlay

  readKeys

  if $keys["up"]
    bg.move(0, 2)
    obstacles.move(0, 2)

    if obstacles.collide(angel) # If we collide, we'll cancel the move
      bg.move(0, -2)
      obstacles.move(0, -2)
    end

    angel.setDirection($directions["UP"])
  end

  if $keys["down"]
    bg.move(0, -2)
    obstacles.move(0, -2)
    
    if obstacles.collide(angel)
      bg.move(0, 2)
      obstacles.move(0, 2)
    end

    angel.setDirection($directions["DOWN"])
  end
  
  if $keys["left"]
    bg.move(2, 0)
    obstacles.move(2, 0)

    if obstacles.collide(angel)
      bg.move(-2, 0)
      obstacles.move(-2, 0)
    end

    angel.setDirection($directions["LEFT"])
  end

  if $keys["right"]
    bg.move(-2, 0)
    obstacles.move(-2, 0)

    if obstacles.collide(angel)
      bg.move(2, 0)
      obstacles.move(2, 0)
    end

    angel.setDirection($directions["RIGHT"])
  end

  startDraw

  clearScreen # The map is already ugly...

  bg.draw
  obstacles.draw
  angel.draw
  endDraw
  
  sync
end

stopGfx
stopLib
