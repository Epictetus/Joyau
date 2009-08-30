initLib
initGfx

bg = GameMap.new("bg_tile.png")
obstacles = GameMap.new("obs_tile.png")

bg.pos = Point.new(0, 0)
obstacles.pos = Point.new(0, 0)

bg.setTileSize(32, 32)
obstacles.setTileSize(32, 32)

obstacles.collisionH = 10

# let's draw a really ugly background !

for i in 0..20
  bg.addElem(-1, 0, 0, i * 32, 0)
  bg.addElem Tile.new(-1, 32, 0, i * 32, 32)
end

# he ! The used tilesets aren't right, are they ?
bg.each_tile { |tile|
  tile.tileset += 1
  tile
}

# fixed :) ( I just had to show each_tile )

obstacles << Tile.new(0, 0, 0, 0, 0)
obstacles.addElem(0, 0, 0, 5 * 32, 5 * 32)

angel = Sprite.new("angel.png")
angel.setAnim(3, 4)

angel.direction = Sprite::DOWN
angel.setPos(240 - angel.w / 2, 136 - angel.h / 2)

skip = false

while mayPlay
  Pad.update

  if Pad.held? Pad::UP
    bg.move(0, 2)
    obstacles.move(0, 2)

    angel.direction = Sprite::UP
  end

  if Pad.held? Pad::DOWN
    bg.move(0, -2)
    obstacles.move(0, -2)
    
    angel.direction = Sprite::DOWN
  end
  
  if Pad.held? Pad::LEFT
    bg.move(2, 0)
    obstacles.move(2, 0)

    angel.direction = Sprite::LEFT
  end

  if Pad.held? Pad::RIGHT
    bg.move(-2, 0)
    obstacles.move(-2, 0)

    angel.direction = Sprite::RIGHT
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
