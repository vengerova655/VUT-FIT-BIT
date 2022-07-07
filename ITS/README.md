# ITS Projekt 1

- **Autor:** Veronika Vengerova (xvenge01)
- **Datum:** 2021-04-19

## Matice pokrytí artefaktů


| Artifact ID  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 |
|--------------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|----|----|----|----|----|
| P_UC         | x |   |   |   | x |   |   |   |   | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  |    |
| P_LIST_UC    | x |   |   |   |   |   |   |   |   | x  |    |    |    |    |    |    |    |    | x  |    | x  |
| P_ES         |   | x |   |   |   |   |   |   |   |    |    |    |    |    |    |    |    |    |    |    |    |
| P_LIST_O     |   |   | x |   |   |   |   |   |   |    |    |    |    |    |    |    | x  |    |    | x  |    |
| P_O          |   |   | x |   |   | x | x | x |   |    |    |    |    |    |    |    |    |    |    |    |    |
| P_R          |   |   |   | x |   |   |   |   | x |    |    |    |    |    |    |    |    |    |    |    |    |
| ES_LINK_UC   |   |   |   |   | x |   |   |   |   |    |    |    |    |    |    |    |    |    |    |    |    |
| PA_LINK_UC   |   |   |   |   |   | x |   |   |   |    |    |    |    |    |    |    |    |    |    |    |    |
| ES_LINK_PA   |   |   |   |   |   |   | x |   |   |    |    |    |    |    |    |    |    |    |    |    |    |
| O_LINK_ES    |   |   |   |   |   |   |   | x |   |    |    |    |    |    |    |    |    |    |    |    |    |
| R_LINK_ES    |   |   |   |   |   |   |   |   | x |    |    |    |    |    |    |    |    |    |    |    |    |
| STATE        |   |   |   |   |   |   |   |   |   |    |    |    |    |    |    |    |    |    | x  | x  | x  |
| REM_O        |   |   |   |   |   |   |   |   |   |    |    |    |    |    |    |    | x  |    |    |    |    |
| REM_ES       |   |   |   |   |   |   |   |   |   |    |    |    |    |    |    |    |    | x  |    |    |    |
| REM_P_FR_UC  |   |   |   |   |   |   |   |   |   |    |    |    |    |    |    | x  |    |    |    |    |    |
| REM_ES_FR_UC |   |   |   |   |   |   |   |   |   |    |    |    |    |    | x  |    |    |    |    |    |    |

 <br />


Popis artefaktů:

| ArtifactId   | Artifact Description                                          |
|--------------|---------------------------------------------------------------|
| P_UC         | Web page for viewing use case                                 |
| P_LIST_UC    | Web page for viewing list of use cases                        |
| P_ES         | Web page for viewing evaluation scenario                      |
| P_LIST_O     | Web page for viewing list of  organisations                   |
| P_O          | Web page for viewing organisation                             |
| P_R          | Web page for viewing requirement                              |
| ES_LINK_UC   | Checking relation evaluation scenario to use case             |
| PA_LINK_UC   | Checking relation of use case to use case provider            |
| ES_LINK_PA   | Checking relation of evaluation scenario and partner          |
| O_LINK_ES    | Checking relation owner to evaluation scenario                |
| R_LINK_ES    | Checking requirements of evaluation scenario and requirements |
| P_LIST_O     | Web page for viewing list of organisations                    |
| STATE        | Checking state                                                |
| REM_O        | Removing organisation                                         |
| REM_ES       | Removing evaluation scenario                                  |
| REM_P_FR_UC  | Removing partner from usecase                                 |
| REM_ES_FR_UC | Removing evaluation scenario from use case                    |

<br />

Popis scénářů:

| Test Scenario                                                                  | Number |
|--------------------------------------------------------------------------------|--------|
| Scenario: Add new usecase                                                      | 1      |
| Scenario: Add new evaluation scenario                                          | 2      |
| Scenario: Add new organization                                                 | 3      |
| Scenario: Add new requirement                                                  | 4      |
| Scenario: Checking usecase link to evaluation scenario                         | 5      |
| Scenario: Checking usecase link with use case provider                         | 6      |
| Scenario: Checking usecase link with partners                                  | 7      |
| Scenario: Checking evaluation scenario owner                                   | 8      |
| Scenario: Checking requirements of evaluation scenarios                        | 9      |
| Scenario: Editing use case name                                                | 10     |
| Scenario: Adding evaluation scenario to existing usecase                       | 11     |
| Scenario: Changing use case provider of existing usecase                       | 12     |
| Scenario: Adding partner to existing usecase                                   | 13     |
| Scenario: Changing the use case description                                    | 14     |
| Scenario: Removing evaluation scenario from use case                           | 15     |
| Scenario: Removing Partner from use case partners list                         | 16     |
| Scenario: Removing organisation                                                | 17     |
| Scenario: Removing Evaluation Scenario                                         | 18     |
| Scenario: Changing the state of usecase to public                              | 19     |
| Scenario: Changing private the state of organisation linked to public use case | 20     |
| Scenario: Changing the state of item to private                                | 21     |

 <br />

## Matice Feature-Test

| Feature file                   | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 |
|--------------------------------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|----|----|----|----|----|----|----|
| Creating_items.feature         | x | x | x | x |   |   |   |   |   |    |    |    |    |    |    |    |    |    |    |    |    |
| Checking_usecase_links.feature |   |   |   |   | x | x | x | x | x |    |    |    |    |    |    |    |    |    |    |    |    |
| Editing.feature                |   |   |   |   |   |   |   |   |   | x  | x  | x  | x  | x  |    |    |    |    |    |    |    |
| Removing_items.feature         |   |   |   |   |   |   |   |   |   |    |    |    |    |    | x  | x  | x  | x  |    |    |    |
| State_changing.feature         |   |   |   |   |   |   |   |   |   |    |    |    |    |    |    |    |    |    | x  | x  | x  |


 <br />

