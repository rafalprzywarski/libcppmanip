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
  $SOURCE = source
end

When /^I run function extraction for "(.*?)" with name "(.*?)"$/ do |phrase, functionName|
  step "I run function extraction from \"#{phrase}\" to \"#{phrase}\" with name \"#{functionName}\""
end

def runExtractFunction args
  File.open(SOURCE_FILE, "w") { |f| f.write $SOURCE }
  $cppmanip_output = %x(#{RUNNER_BUILD_DIRECTORY}/cppmaniprunner_extract_function #{args} 2>&1)
  $?.should eq(0), "cppmanip failed with error \'#{$?}\': #{$cppmanip_output}"
  @replacements = loadReplacementsFromXml(REPLACEMENTS_FILE) if File.file?(REPLACEMENTS_FILE)
  @error = loadError(ERROR_FILE) if @replacements.nil?
end

When /^I run function extraction from "(.*?)" to "(.*?)" with name "(.*?)"$/ do |startPhrase, endPhrase, functionName|
  startOffset, endOffset = rangeFromPhrases startPhrase, endPhrase, $SOURCE
  runExtractFunction "#{SOURCE_FILE} #{functionName} #{startOffset} #{endOffset}"
end

When /^I run two function extractions for "(.*?)" with name "(.*?)" and for "(.*?)" with name "(.*?)"$/ do |phrase1, functionName1, phrase2, functionName2|
  startOffset1, endOffset1 = rangeFromPhrases phrase1, phrase1, $SOURCE
  startOffset2, endOffset2 = rangeFromPhrases phrase2, phrase2, $SOURCE
  runExtractFunction "#{SOURCE_FILE} #{functionName1} #{startOffset1} #{endOffset1} #{functionName2} #{startOffset2} #{endOffset2}"
end

Then /^it should fail with a message "(.*?)"$/ do |expectedMessage|
  @error.should eq(expectedMessage), "cppmanip should have failed with message: #{expectedMessage}"
end

Given /^libcppmanip repository$/ do
    #we are in it
end

When /^I install it$/ do
  pending # express the regexp above with the code you wish you had
end

Then /^another project using it should compile and link$/ do
  pending # express the regexp above with the code you wish you had
end

Then /^there should be (\d+) changes$/ do |changeCount|
  @replacements.count.should eq(changeCount)
end

Then /^there should be an insertion:$/ do |insertionText|
  @replacements.index { |r| r.isInsertion && r.text == insertionText }.should_not be_nil, "insertion with \'#{insertionText}\' not found in #{@replacements}"
end

Then /^there should be an insertion before "(.*?)":$/ do |before, insertionText|
  @replacements.index { |r| r.isInsertionBefore(before, $SOURCE) && r.text == insertionText }.should_not be_nil
end

Then /^there should be a replacement with "(.*?)"$/ do |replacementText|
  @replacements.index { |r| r.text == replacementText }.should_not be_nil, "replacement with \'#{replacementText}\' not found in #{@replacements}"
end

Then /^there should be a replacement from "(.*?)" to "(.*?)" with "(.*?)"$/ do |from, to, replacementText|
  @replacements.index { |r|
    r.text == replacementText && r.isFrom(from, $SOURCE) && r.isTo(to, $SOURCE)
  }.should_not be_nil, "replacement with \'#{replacementText}\' not found in #{@replacements}"
end

Then /^there should be a replacement for "(.*?)" with "(.*?)"$/ do |phrase, replacementText|
  step "there should be a replacement from \"#{phrase}\" to \"#{phrase}\" with \"#{replacementText}\""
end
