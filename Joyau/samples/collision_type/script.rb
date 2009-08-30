initLib
initGfx

types = [ CollisionType.new(false, true, false, false, false),
          CollisionType.new(false, false, true, false, false),
          CollisionType.new(false, false, false, true, false),
          CollisionType.new(false, false, false, false, true) ]

obstacles = GameMap.new("obs_tile.png")
obstacles.pos = Point.new(0, 0)
obstacles.setTileSize(32, 32)

obstacles << Tile.new(0, 0, 0, 0, 0, types[0])
obstacles << Tile.new(0, 0, 0, 32, 32, types[1])
obstacles << Tile.new(0, 0, 0, 64, 64, types[2])
obstacles << Tile.new(0, 0, 0, 5 * 32, 5 * 32, types[3])

angel = Sprite.new("angel.png")
angel.setAnim(3, 4)

angel.direction = Sprite::DOWN
angel.setPos(240 - angel.w / 2, 136 - angel.h / 2)

skip = false

while mayPlay
  Pad.update

  if Pad.held? Pad::UP
    obstacles.move(0, 2)
    angel.direction = Sprite::UP
  end

  if Pad.held? Pad::DOWN
    obstacles.move(0, -2)
    angel.direction = Sprite::DOWN
  end
  
  if Pad.held? Pad::LEFT
    obstacles.move(2, 0)
    angel.direction = Sprite::LEFT
  end

  if Pad.held? Pad::RIGHT
    obstacles.move(-2, 0)
    angel.direction = Sprite::RIGHT
  end

  if obstacles.collide(angel)
    obstacles.cancelMove
  end

  if !skip
    startDraw

    clearScreen # The map is already ugly...

    obstacles.draw
    angel.draw

    endDraw
  end
  
  skip = sync
end

stopGfx
stopLib
