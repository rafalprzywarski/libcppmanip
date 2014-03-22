require 'rspec'
require './buildpath'
require 'replacements'
require 'errors'
require 'ranges'

SOURCE_FILE="source.cpp"
REPLACEMENTS_FILE="replacements.xml"
ERROR_FILE="errors.xml"

Transform /^(-?\d+)$/ do |number|
  number.to_i
end

After do
  [ SOURCE_FILE, REPLACEMENTS_FILE, ERROR_FILE ].each do |f|
    File.delete(f) if File.file?(f)
  end
end

Given /^source code:$/ do |source|
  @source = source
end

When /^I run function extraction for "(.*?)" with name "(.*?)"$/ do |phrase, functionName|
  step "I run function extraction from \"#{phrase}\" to \"#{phrase}\" with name \"#{functionName}\""
end

def runExtractFunction args
  File.open(SOURCE_FILE, "w") { |f| f.write @source }
  output = %x(#{RUNNER_BUILD_DIRECTORY}/cppmaniprunner_extract_function #{args} 2>&1)
  $?.should eq(0), "cppmanip failed with error \'#{$?}\': #{output}"
  @replacements = loadReplacementsFromXml(REPLACEMENTS_FILE) if File.file?(REPLACEMENTS_FILE)
  @error = loadError(ERROR_FILE) if @replacements.nil?
end

When /^I run function extraction from "(.*?)" to "(.*?)" with name "(.*?)"$/ do |startPhrase, endPhrase, functionName|
  startLoc, endLoc = rangeFromPhrases startPhrase, endPhrase, @source
  runExtractFunction "#{SOURCE_FILE} #{functionName} #{startLoc.row} #{startLoc.col} #{endLoc.row} #{endLoc.col}"
end

When /^I run two function extractions for "(.*?)" with name "(.*?)" and for "(.*?)" with name "(.*?)"$/ do |phrase1, functionName1, phrase2, functionName2|
  startLoc1, endLoc1 = rangeFromPhrases phrase1, phrase1, @source
  startLoc2, endLoc2 = rangeFromPhrases phrase2, phrase2, @source
  runExtractFunction "#{SOURCE_FILE} #{functionName1} #{startLoc1.row} #{startLoc1.col} #{endLoc1.row} #{endLoc1.col} #{functionName2} #{startLoc2.row} #{startLoc2.col} #{endLoc2.row} #{endLoc2.col}"
end

Then /^it should fail with a message "(.*?)"$/ do |expectedMessage|
  @error.should eq(expectedMessage), "cppmanip should have failed with message: #{expectedMessage}"
end

def expectNoErrors
  @error.should be_nil, "Failed with error: #{@error}"
end

Then /^there should be (\d+) changes$/ do |changeCount|
  expectNoErrors
  @replacements.count.should eq(changeCount)
end

Then /^there should be an insertion:$/ do |insertionText|
  expectNoErrors
  @replacements.index { |r| r.isInsertion && r.text == insertionText }.should_not be_nil, "insertion with \'#{insertionText}\' not found in #{@replacements}"
end

Then /^there should be an insertion containing "(.*?)"$/ do |text|
  expectNoErrors
  @replacements.index { |r| r.isInsertion && r.text.include?(text) }.should_not be_nil, "insertion containing \'#{text}\' not found in #{@replacements}"
end

Then /^there should be an insertion before "(.*?)":$/ do |before, insertionText|
  expectNoErrors
  @replacements.index { |r| r.isInsertionBefore(before, @source) && r.text == insertionText }.should_not be_nil
end

Then /^there should be a replacement with "(.*?)"$/ do |replacementText|
  expectNoErrors
  @replacements.index { |r| r.text == replacementText }.should_not be_nil, "replacement with \'#{replacementText}\' not found in #{@replacements}"
end

Then /^there should be a replacement from "(.*?)" to "(.*?)" with "(.*?)"$/ do |from, to, replacementText|
  expectNoErrors
  @replacements.index { |r|
    r.text == replacementText && r.isFrom(from, @source) && r.isTo(to, @source)
  }.should_not be_nil, "replacement with \'#{replacementText}\' not found in #{@replacements}"
end

Then /^there should be a replacement for "(.*?)" with "(.*?)"$/ do |phrase, replacementText|
  step "there should be a replacement from \"#{phrase}\" to \"#{phrase}\" with \"#{replacementText}\""
end
