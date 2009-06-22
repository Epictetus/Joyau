class Ghost < Sprite
  
  def draw(hero)
    if @projectils == nil # Same as the projectils...
      @projectils = Array.new
    end
    if hero.getAlpha <= 127 and getAlpha <= 127 #if we're both ghosts
      super()
      for pro in @projectils
        pro.draw
      end
    elsif hero.getAlpha > 127 and getAlpha > 127 #or if we're both solids ones
      super()
      for pro in @projectils
        pro.draw
      end
    end
  end
  
  def play(hero)
    if @projectils == nil
      @projectils = Array.new
    end
    for pro in @projectils
      hasCollided = false
      pro.play
      if hero.collide(pro)
        $life -= 1
        hasCollided = true
      end
      if pro.getCount <= 0 or hasCollided
        @projectils[@projectils.rindex(pro)] = nil # Are pointers missing you ?
        # ( Can't remember about the Ruby references... Not that important )
      end
    end
    @projectils.compact! # don't forget to remove nil item.
    
    axe = rand(2) # Wow, what a great pathfinding algorithm, isn't it ?
    if axe == 1
      if getY > hero.getY
        move(0, -1)
      else
        move(0, 1)
      end
    else
      if getX > hero.getX
        move(-1, 0)
      else
        move(1, 0)
      end
    end

    if @projectils.length < 2 # We'll set a maximum number of projectils : 2.
      # If we're closs of bota on the x axis
      if getX >= hero.getX - 10 and getX <= hero.getX + 10
        # Anyway, we'll create a projectil. So, we'll avoid pressing 
        #C-y too much times...
        pro = Projectil.new
        pro.setPicture("proj.png")
        if getAlpha <= 127
          pro.setAlpha(127)
        end
        pro.setPos(getX, getY)
        if getY > hero.getY
          pro.setDirection($directions["UP"])
        else
          pro.setDirection($directions["DOWN"])
        end
        @projectils.push(pro)
      elsif getY >= hero.getY - 10 and getY <= hero.getY + 10
        # Don"t think we could put this code before.
        # We have to know if we're close of botamon. Before theses conditions,
        # we don't.
        pro = Projectil.new
        pro.setPicture("proj.png")
        if getAlpha <= 127
          pro.setAlpha(127)
        end
        pro.setPos(getX, getY)
        if getX > hero.getX
          pro.setDirection($directions["LEFT"])
        else
          pro.setDirection($directions["RIGHT"])
        end
        @projectils.push(pro)
      end
    end
    
  end
  
end
