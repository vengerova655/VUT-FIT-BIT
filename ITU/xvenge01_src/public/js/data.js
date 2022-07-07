/**
 * @author: Veronika Vengerova(xvenge01)
 */

//sets today so it wont change
const today= new Date();

//sets today to show reminder
const showReminder = [
    {
        "name": "Fero Herceg",
        "year":"2001",
        "month": "1",
        "day": "30",
        "reminder_day":(today.getDate()),
        "reminder_month": (today.getMonth()+1),
        "concept": "",
        "email": "",
        "sensDate": ""
    }
]

//data load at first to show output, these are just to show view, otherwise []
const importantPPL = [
    {
        "name": "Klement Franco",
        "category": "Family",
        "year":"2001",
        "month": "12",
        "day": "8",
        "reminder_day":"",
        "reminder_month": "",
        "important": true,
        "concept": "Vsetko najlepsie, vela zdravi stastia"
    }
,
    {
        "name": "Eva Vargova",
        "category": "Family",
        "year":"2001",
        "month": "1",
        "day": "4",
        "reminder_day":"",
        "reminder_month": "",
        "important": true,
        "concept": "Vsetko najlepsie, vela zdravi stastia"
    },

    {
        "name": "Klaudia Marco",
        "category": "Friends",
        "year":"1992",
        "month": "2",
        "day": "8",
        "reminder_day":"",
        "reminder_month": "",
        "important": true,
        "concept": "Vsetko najlepsie, vela zdravi stastia"
    },

    {
        "name": "Ema Halaszova",
        "category": "Family",
        "year":"2001",
        "month": "5",
        "day": "5",
        "reminder_day":"",
        "reminder_month": "",
        "important": true,
        "concept": "Vsetko najlepsie, vela zdravi stastia"
    }
]

//for categorry we want to add
const categories = [
]

localStorage.setItem("showReminder", JSON.stringify(showReminder));


localStorage.setItem("importantPPL", JSON.stringify(importantPPL));


localStorage.setItem("categories", categories);


function addReminder(R_ReminderJson){
    console.log("Reminders");
    var str = R_ReminderJson.date;
    var p = str.split('-');
    var str2 = R_ReminderJson.reminder;
    var p2 = str2.split('-');   
    var val = (R_ReminderJson.important === "Important");


    var toStore={
        "name": R_ReminderJson.name,
        "year": p[0],
        "month": p[1],
        "day": p[2],
        "reminder_day":p2[2],
        "reminder_month": p2[1],
        "important":val,
        "concept": R_ReminderJson.concept,
        "email": R_ReminderJson.email,
        "sensDate": R_ReminderJson.email_reminder_date
    }

     
    if(R_ReminderJson.reminder !== "" || R_ReminderJson.email_reminder_date!==""){
        console.log(toStore);

        var rem_pars = [];
        rem_pars = JSON.parse(localStorage.getItem("showReminder"));
        console.log(rem_pars);
        rem_pars.push(toStore);
        localStorage.setItem("showReminder", JSON.stringify(rem_pars));
    }


    if(R_ReminderJson.important === "Important"){
        console.log(toStore);

        var rem_pars = [];
        rem_pars = JSON.parse(localStorage.getItem("importantPPL"));
        console.log(rem_pars);
        rem_pars.push(toStore);
        localStorage.setItem("importantPPL", JSON.stringify(rem_pars));
        writeImportant();
    }
}

const getTodaysBirthdays = [
    {
        "name": "Klementina francova",
        "category": "Family",
        "year":"2001",
        "month": "12",
        "day": today.getDate(),
        "reminder_day":"",
        "reminder_month": "",
        "koncept": "Vsetko najlepsie, vela zdravi stastia"
    }
]


//to show notification for todays birthdays
function getTodaysReminder(){
    var x =[];

    var rem_pars = [];
    rem_pars = JSON.parse(localStorage.getItem("showReminder"));
    rem_pars.forEach(element => {
        if(element.reminder_day == today.getDate()){
            x.push(element);
        }
        else{
            if(element.day == today.getDate()){
                x.push(element);
            }
        }
    });
    return x;
}


function getTodaysB(){
    var x =[];
    getTodaysBirthdays.forEach(element => {
    
        if(element.day === today.getDate()){
            x.push(element);
        }
    });
    return x;
}

//gets birthdays of people we set that are important up to 3 months ahead
function getImportant(){
    var x =[];
    var m =[];
    m= JSON.parse(localStorage.getItem("importantPPL"));
    m.forEach(element => {
        if(element.important){
            console.log(element.month);
            if(element.month == ((today.getMonth()+1))){
                    console.log("tento mesiac");
                    x.push(element);
            }else{ 
                if(element.month < ((today.getMonth()+1 +3)%12)){
                    console.log("je mensi ako ten datum");
                    x.push(element);
                }
            }
        }
    });
    return x;
}


function sendCategory(x){

    var rem_pars = localStorage.getItem("categories");
    console.log(rem_pars);
    rem_pars=x;
    localStorage.setItem("categories", JSON.stringify(rem_pars));
    return true;
};



function getCategories(){

    var rem_pars = [];
    rem_pars = JSON.parse(localStorage.getItem("categories"));
    return rem_pars;
};


//function to send email with wish
function sendEmail(sendingJSON){
    return true;
}