Feature: Checking usecase links

Scenario: Checking usecase link to evaluation scenario
	Given we have evaluation scenario in use case
	When we click on the evaluation scenario
	Then the evaluation scenario page is shown


Scenario: Checking usecase link with use case provider
	Given we have usecase with use case provider
	When we click on the use case provider
	Then the use case provider's page is shown


Scenario: Checking usecase link with partners
	Given we have usecase with partners
	When we click on the partner
	Then the partner's page is shown


Scenario: Checking evaluation scenario owner
	Given evaluation scenario owner si linked to usecase
	When we click on the owner
	Then the owner's page is shown

Scenario: Checking requirements of evaluation scenarios
	Given we have evaluation scenario with requirement
	When we click on the requirement
	Then the page of requirement is shown
