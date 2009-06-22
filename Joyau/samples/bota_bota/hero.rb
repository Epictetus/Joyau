class Hero < Sprite

  def draw
    super()
    if @projectils == nil
      @projectils = Array.new
    end
    for pro in @projectils
      pro.draw
    end
  end
    
  def play(advs)
    if @projectils == nil
      @projectils = Array.new
    end
    for pro in @projectils
      hasCollided = false
      pro.play
      for adv in advs
        if adv.collide(pro)
          $score += 1
          hasCollided = true
        end
      end
      if pro.getCount <= 0 or hasCollided
        @projectils[@projectils.rindex(pro)] = nil # I'd rather user 
                                                   # pro = nil but...
      end
    end
    @projectils.compact!

    readKeys # Ahem, without that we don't need the folowing conditions...
    
    if $keys["up"]
      move(0, -2) # We move it, of course, ...
      setDirection($directions["UP"]) # But we also set a direction.
      # That's for the projectils.
    end
    if $keys["down"]
      move(0, 2)
      setDirection($directions["DOWN"])
    end
    if $keys["right"]
      move(2, 0)
      setDirection($directions["RIGHT"])
    end
    if $keys["left"]
      move(-2, 0)
      setDirection($directions["LEFT"])
    end
    if $keys["cross"]
      if getAlpha - 2 > 20 # Hey, I want to see Bota !
        setAlpha(getAlpha - 2)
      end
    end
    if $keys["triangle"]
      if getAlpha + 2 < 256 # What would happen with alpha = 500 ?
        # Really I don't know...
        setAlpha(getAlpha + 2)
      end
    end
    if $keys["square"] # NB : I know the user doesn't 
      #see there are two projectils. If you want to see it, add a timer or
      #whatever you want...
      if @projectils.length < 2
        pro = Projectil.new
        pro.setPicture("proj.png")
        if getAlpha <= 127
          pro.setAlpha(127)
        end
        pro.setPos(getX, getY)
        pro.setDirection(getDirection)
        @projectils.push(pro)
      end
    end
  end
end
