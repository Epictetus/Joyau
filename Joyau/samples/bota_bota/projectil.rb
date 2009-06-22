class Projectil < Sprite
  
  def play
    if @count == nil # I've issues with initialize, thus I do that so...
                     # Hope it's not that slow...
      @count = 100
    end
    @count -= 1
    if getDirection == $directions["RIGHT"] # Hey, why do you think 
                                            # there is a such hash ?
      move(3, 0)
    elsif getDirection == $directions["LEFT"]
      move(-3, 0)
    elsif getDirection == $directions["DOWN"]
      move(0, 3)
    elsif getDirection == $directions["UP"]
      move(0, -3)
    end
  end

  def getCount # I had so much problem with theses variables I don't want to 
               # try attr_reader on it... 
    return @count
  end
  
end
