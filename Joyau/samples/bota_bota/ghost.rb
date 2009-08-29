class Ghost < Sprite
  def draw(hero)
    if (hero.alpha <= 127 and alpha <= 127) or
        (hero.alpha > 127 and alpha > 127) # if we have both the same opacity
      super()
      @projectils.each { |pro| pro.draw }
    end
  end
  
  def play(hero)
    if @projectils == nil
      @projectils = []
    end

    @projectils.each { |pro|
      pro.hasCollided = false
      pro.play

      if hero.collide(pro)
        $life -= 1
        pro.hasCollided = true
      end
    }
    @projectils.reject! { |pro| pro.count <= 0 || pro.hasCollided }
    
    axe = rand(2) # Wow, what a great pathfinding algorithm, isn't it ?
    if axe == 1
      if y > hero.y
        move(0, -1)
      else
        move(0, 1)
      end
    else
      if x > hero.x
        move(-1, 0)
      else
        move(1, 0)
      end
    end

    if @projectils.length < 2 # We'll set a maximum number of projectils : 2.
      # If we're closs of bota on the x axis
      if x >= hero.x - 10 and x <= hero.x + 10
        # Anyway, we'll create a projectil. So, we'll avoid pressing 
        #C-y too much times...
        pro = Projectil.new("proj.png")
        if alpha <= 127
          pro.alpha = 127
        end
        pro.pos = Point.new(x, y)
        if y > hero.y
          pro.direction = Sprite::UP
        else
          pro.direction = Sprite::DOWN
        end
        @projectils << pro
      elsif y >= hero.y - 10 and y <= hero.y + 10
        # Don't think we could put this code before.
        # We have to know if we're close of botamon. Before theses conditions,
        # we don't.
        pro = Projectil.new("proj.png")
        if alpha <= 127
          pro.alpha = 127
        end
        pro.pos = Point.new(x, y)
        if x > hero.x
          pro.direction = Sprite::LEFT
        else
          pro.direction = Sprite::RIGHT
        end
        @projectils << pro
      end
    end
    
  end
    
end
