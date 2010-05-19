require 'spec'

[:pressed, :held, :released].each do |state|
  Spec::Matchers.define "be_#{state}".to_sym do
    match do |key|
      Joyau::Pad.send("#{state}?".to_sym, key)
    end
    
    failure_message_for_should do |key|
      "The key `0x#{key.to_s(16)}' should be #{state}, but is not."
    end
    
    failure_message_for_should_not do |key|
      "The key `0x#{key.to_s(16)}' shouldn't be #{state}."
    end
  end
end

Spec::Matchers.define :be_at do |x, y|
  match do |object|
    object.x == x and object.y == y
  end

  failure_message_for_should do |object|
    "#{object} should be at (#{x},#{y}) but is at (#{object.x},#{object.y})"
  end
  
  failure_message_for_should_not do |object|
    "#{object} shouldn't be at (#{x},#{y})."
  end
end

Spec::Matchers.define :collide_with do |second|
  match do |first|
    first.collide(second)
  end

  failure_message_for_should do |first|
    "#{first} should collide with #{second}, but is not."
  end
  
  failure_message_for_should_not do |first|
    "#{first} shouldn't collide with #{second}."
  end
end

Spec::Matchers.define :be_on do |x, y|
  match do |object|
    object.isOn(x, y)
  end

  failure_message_for_should do |object|
    "#{object} should be on (#{x},#{y}) but is not."
  end
  
  failure_message_for_should_not do |object|
    "#{object} shouldn't be on (#{x},#{y})."
  end
end

Spec::Matchers.define :have_moved_of do |x, y|
  match do |object|
    object.movedX == x and object.movedY == y
  end

  failure_message_for_should do |object|
    "#{object} should have moved by (#{x},#{y}) but has moved by \
(#{object.movedX},#{object.movedY})."
  end
  
  failure_message_for_should_not do |object|
    "#{object} shouldn't have moved by (#{x},#{y})"
  end
end

Spec::Matchers.define :have_moved do
  match do |object|
    object.movedX != 0 and object.movedY != 0
  end

  failure_message_for_should do |object|
    "#{object} should have moved"
  end
  
  failure_message_for_should_not do |object|
    "#{object} shouldn't have moved"
  end
end
