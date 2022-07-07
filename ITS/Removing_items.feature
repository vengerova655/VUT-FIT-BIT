Feature: Removing items

Scenario: Removing evaluation scenario from use case
	Given we are able to change use case
	When we remove evaluation scenario from list
	Then the link to evaluation scenario is removed from use case page

Scenario: Removing Partner from use case partners list
	Given we are able to change use case
	When we remove the partner from list of partners
	Then the link to partner is removed from use case page

Scenario: Removing organisation
	Given we are able to edit organisation
	And organisation is owner of evaluation scenario
	When we remove organisation
	Then the organisation is removed from the list of organisations
	And the link to it is also removed from use case owner


Scenario: Removing Evaluation Scenario
	Given we are able to edit evaluation scenario
	And the evaluation scenario is linked to use case
	When we remove evaluation scenario
	Then the evaluation scenario is removed
	And the link to it in use case is also removed

