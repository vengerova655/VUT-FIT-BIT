Feature: Editing items

Background:
	Given: we are able to change use case

Scenario: Editing use case name
	When we change the name of use case
	Then the name of the use case page is changed
	And the name of use case at the page of list of use cases is changed

Scenario: Adding evaluation scenario to existing usecase
	When we add evaluation scenario
	Then the evaluation scenario is shown in the list of evaluation scenarios of usecase
	

Scenario: Changing use case provider of existing usecase
	When we change use case provider
	Then the new use case provider is shown in use case page
	

Scenario: Adding partner to existing usecase
	When we add partner to list of partners
	Then the partner is shown in the list of partners of usecase
	
Scenario: Changing the use case description
	When we edit the use case description
	Then the use case description on page is updated


