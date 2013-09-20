require 'rspec'
require 'OffsetConverter'

describe :OffsetCoverter do
  it "should return offset in the first line" do
    conv = OffsetConverter.new("xxxxx\nx\nxxxxxxxxx")
    loc = conv.getLocationFromOffset(5)
    loc.col.should eq(5)
    loc.row.should eq(0)
    conv.getLocationFromOffset(0).col.should eq(0)
  end
end