Feature: Creating items

Background:
    Given we are logged in as producent
    And web browser is at the page we want to add new item to


Scenario: Add new usecase
    When we create new case
    Then the use case is created
    And the use case is added to the page with list of use cases

Scenario: Add new evaluation scenario
    When we create new evaluation scenario
    Then the evaluation scenario is added to the page


Scenario: Add new organization
    When we create new organization
    Then the organization case is created
    And added to the organizations list page


Scenario: Add new requirement
    When we create new requirement
    Then the requirement is added to the page




