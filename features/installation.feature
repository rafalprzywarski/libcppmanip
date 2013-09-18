Feature: As a plug-in writer I want to be able to install this library
    @wip
    Scenario: Installation
        Given libcppmanip repository
        When I install it
        Then another project using it should compile and link
