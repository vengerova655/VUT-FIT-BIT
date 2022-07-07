Feature: Changing state

Background: 
	Given we can change the state of items
	
Scenario: Changing the state of usecase to public
	When publish the use case
	Then the page of usecase is visible for consuments
	And the usecase is visible in list of usecases

Scenario: Changing the private state of organisation linked to public use case 
	Given the private organisation is linked to public use case
	When publish organisation
	Then the organisation is visible in use case for consumer
	And the organisation is visible in organisation list for consumer

Scenario: Changing the state of usecase to private
	Given there is usecase created that is public
	When we return state to private
	Then the usecase is no longer visible in user cases list page
	
	
