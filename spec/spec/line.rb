require 'joyau_helper'
require "spec/drawable.rb"

describe Joyau::Line do
  before :each do
    @drawable = Joyau::Line.new
  end

  it_should_behave_like "all drawables"

  it "should compute smart collisions" do
    @drawable.pos   = Joyau::Point.new(0, 0)
    @drawable.point = Joyau::Point.new(480, 272)

    @drawable.should_not collide_with(Joyau::DrawableRect.new(0, 272, 40, 230))
    @drawable.should collide_with(Joyau::DrawableRect.new(100, 40, 200, 200))
  end
end
