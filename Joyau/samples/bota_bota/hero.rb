class Hero < Sprite

  def draw
    super()
    for pro in @projectils
      pro.draw
    end
  end
    
  def play(advs)
    if @projectils == nil
      @projectils = Array.new
    end

    @projectils.each { |pro|
      pro.hasCollided = false
      pro.play

      advs.each { |adv|
        if adv.collide(pro)
          $score += 1
          pro.hasCollided = true
        end
      }
    }

    @projectils.reject! { |pro| pro.count <= 0 || pro.hasCollided }

    readKeys # Ahem, without that we don't need the folowing conditions...
    
    if $keys["up"]
      move(0, -2) # We move it, of course, ...
      # But no direction = !
      # autodir = true ;)
    end
    if $keys["down"]
      move(0, 2)
    end
    if $keys["right"]
      move(2, 0)
    end
    if $keys["left"]
      move(-2, 0)
    end

    if $keys["cross"]
      if self.alpha - 2 > 20 # Hey, I want to see Bota !
        self.alpha -= 2
      end
    end
    if $keys["triangle"]
      if self.alpha + 2 < 256 # What would happen with alpha = 500 ?
        self.alpha += 2
      end
    end

    if $keys["pressed_square"]
      if @projectils.length < 2
        pro = Projectil.new("proj.png")
        if self.alpha <= 127
          pro.alpha = 127
        end
        pro.pos = Point.new(x, y)
        pro.direction = direction
        @projectils << pro
      end
    end

  end
end
