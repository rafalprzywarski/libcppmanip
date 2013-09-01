require 'rspec'
require './buildpath'

SOURCE_FILE="source.cpp"

class RowColumn
  attr_accessor :row, :col
  def initialize row, col
    @row, @col = row, col
  end
end

def locationFromOffset offset, source
  leading = source[0..offset]
  offset -= (leading.rindex("\n") || -1) + 1
  RowColumn.new leading.count("\n") + 1, offset + 1
end

def rangeFromPhrases startPhrase, endPhrase, source
  startIndex = $SOURCE.index(startPhrase)
  endIndex = $SOURCE.index(endPhrase)
  startIndex.should_not be_nil, "\"#{startPhrase}\" not found in the source code"
  endIndex.should_not be_nil, "\"#{endPhrase}\" not found in the source code"
  startIndex.should be < endIndex, "\"#{startPhrase}\" must be before \"#{endPhrase}\""
  endLoc = locationFromOffset(endIndex, source)
  endLoc.col += endPhrase.length
  return locationFromOffset(startIndex, source), endLoc
end

After do
  File.delete(SOURCE_FILE)
end

Given /^source code:$/ do |source|
  $SOURCE = source
end

When /^I run method extraction from "(.*?)" to "(.*?)" with name "(.*?)"$/ do |startPhrase, endPhrase, methodName|
  startLoc, endLoc = rangeFromPhrases startPhrase, endPhrase, $SOURCE
  File.open(SOURCE_FILE, "w") { |f| f.write $SOURCE }
  output = %x(#{BUILD_DIRECTORY}/cppmanip source.cpp extract_method #{methodName} #{startLoc.row} #{startLoc.col} #{endLoc.row} #{endLoc.col})
  $?.should eq(0), "cppmanip failed with error code #{$?}: #{output}"
end

Then /^final source code should be:$/ do |expectedSource|
  File.read(SOURCE_FILE).should eq(expectedSource)
end

Then /^final source code should contain:$/ do |expectedSource|
  File.read(SOURCE_FILE).should include(expectedSource)
end
