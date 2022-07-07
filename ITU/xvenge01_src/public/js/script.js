/**
 * @author: Veronika Vengerova(xvenge01)
 */
let colorFamily = "#b6869b";
let colorFriends = "#bca9e7";
const months = ["January", "February", "March", "April", "May", "June",
"July", "August", "September", "October", "November", "December"
];

const dt = new Date();
const currDate = new Date();

/**
 * Function shows the month and year at the top of the application
 * @param {number} month 
 * @param {number} year 
 */
function renderMonth(month, year){
    document.getElementById('MonthDisplay').innerText = month;
    document.getElementById('YearDisplay').innerText = year;
};

//Constants and variables for elements of html for easier work with them inside of funcitons

const modalbkgrnd = document.getElementById("modalbkgrndscreen");
//Modal with form that adds category
const modal_Cat = document.getElementById("modal-category");
const addCategory = document.getElementById("AddCategory");
const span6 = document.getElementsByClassName("close")[4];//close button for modal category
//Modal to add new Birthday
const modal = document.getElementById("modal-newBirthday");
const modalR = document.getElementById("modal-Reminder");
const todaybkgrnd = document.getElementById("modalbkgrndtoday");
const btn = document.getElementById("AddBirthday");
const btnsendRem = document.getElementById("RemindButton");
const span = document.getElementsByClassName("close")[0];
const span3 = document.getElementsByClassName("close")[1];


const s_remquit = document.getElementsByClassName("close")[2];
const btnSenWish = document.getElementById("sendRButton");

//Modal for notification for todays birthdays 
const todayNotif = document.getElementById("s_today");
const btnSentWish = document.getElementById("sendtButton");
const span5 = document.getElementsByClassName("close")[3];



/**
 * Function reads from form inputed data and sends them to store and updates view
 */
function saveCategory(){
    var x = document.getElementById("F_Category").value;
    if(x==""){
        $('.error')[2].innerText = "Name of Category must be given";
    }
    else{
        $('.error')[2].innerText = "";
        if( sendCategory(x)){
            $( "#saved" ).text( "Category Added..." ).show().fadeOut( 5000 );
            loadCategory(x);
        }
        else{
            $( "#saved" ).text( "Category not added succesfully..." ).show().fadeOut( 5000 );
      
        }
        closeCategoryModul();
    }
}

/**
 * hides category modul
 */
function closeCategoryModul(){
    modal_Cat.style.display="none";
    modalbkgrnd.style.display="none";
    document.getElementById("F_Category").value = "";
}

addCategory.onclick = function(){
    modal_Cat.style.display="block";
    modalbkgrnd.style.display="block";
}

/**
 * Adds new category to view
 * @param {String} x the name of new category 
 */
function loadCategory(x){
    var randomColor = "whitesmoke";
    var cat = '<label  style="border: 0.2rem solid var(--color-darkgreen); borderbackground-color:'+randomColor+'"><input type="checkbox" id="is'+x+'Checked" value="' +x +'" checked/> ' +x+'</label>';
    document.getElementById("show-category").innerHTML += cat;
    var cat2 = '<label  style="border: 0.2rem solid var(--color-darkgreen); borderbackground-color:'+randomColor+'"><input type="radio" id="R_is'+x+'" name="category" value="' +x +'" checked/> ' +x+'</label>';
    
    document.getElementById("checkboxRadio").innerHTML += cat2;
}

/**
 * Hides Category Modul
 */
span6.onclick = function(){
    closeCategoryModul();
}

/**
 * Sends inputed wish to inputed email on submit 
 */
function sendTodayWish(){
    if (document.forms["todayForm"]["T_email"].value == "")
        {
            $('#showEmailError').innerHTML = "Email must be given";
        
        }
        else{
        }
            var T_email =  $('#T_email').val();
            var s_tText =  $('#s_tText').val();

            var sendingJSON = {
                "email": R_email,
                "concept":B_WishConcept
            };

            if(sendEmail(sendingJSON)){
                $( "#saved" ).text( "Birthday wish was sent..." ).show().fadeOut( 5000 );
                closeToday();
            }else{
                $( "#saved" ).text( "Error while sending..." ).show().fadeOut( 5000 );
            }

};



todaybkgrnd.onclick = function(){
    closeToday();
}

span5.onclick = function(){
    closeToday();
}

function closeToday(){
    todayNotif.style.display = "none";
    todaybkgrnd.style.display ="none";

}

btnSenWish.onclick = function(){
    closeSRemindModul();
    $( "#loading" ).text( "Concept wish saved" ).show().fadeOut( 3000 );

}
s_remquit.onclick = function(){
    closeSRemindModul();
}

function closeRemindModul(){
    modalR.style.visibility = "hidden";
}

/**
 * Closes notification for todays stored notification
 */
function closeSRemindModul(){
    modalbkgrnd.style.display = "none";
    reminderNotif.style.display = "none";
}

span3.onclick = function(){
    document.getElementById("ch_ReminderDate").value="";
    document.getElementById("R_email").value="";
 
    document.getElementById("B_WishConcept").value="";
 
    closeRemindModul();
}

btnsendRem.onclick = function(){
    closeRemindModul();
}

btn.onclick = function() {
    modalbkgrnd.style.display = "block";
    modal.style.display = "block";
};

modalbkgrnd.onclick = function(){
    modal_Cat.style.display="none";
    modal.style.display = "none";
    modalbkgrnd.style.display = "none";
    reminderNotif.style.display = "none";
}

function closeModul(){
    modal.style.display = "none";
    modalbkgrnd.style.display = "none";
    clearBirthdayModel();
};
span.onclick = function() {
    closeModul();
};



function submittingForm(){
    if (document.forms["postForm"]["B_Name"].value == "" || 
        document.forms["postForm"]["B_Date"].value =="")
    {
        if (document.forms["postForm"]["B_Name"].value == ""){
            console.log("faal");
            console.log($('#postForm p')[1]);
            $('#postForm .error')[0].innerText = "Name must be given";
            // $('#postForm p').first().text()
        }
        if(  
            document.forms["postForm"]["B_Date"].value =="") 
        {
            $('#postForm .error')[1].innerText = "The whole date must be given";
        }
    }
    else{
        var B_Name =  $('#B_Name').val();
        var B_ReminderDate =  $('#B_ReminderDate').val();
        var B_Date =  $('#B_Date').val();
        var B_Category = $("#postForm input[name='category']:checked").val();
        var B_Important = $("#postForm input[name='importance']:checked").val();
    
        var ch_ReminderDate =  $('#ch_ReminderDate').val();
        var R_email =  $('#R_email').val();
        var B_WishConcept =  $('#B_WishConcept').val();
      
        var sendingJSON = {
            "name": B_Name,
            "reminder": B_ReminderDate,
            "category": B_Category,
            "date":B_Date
        };
        var storeJSON = {
            "name": B_Name,
            "reminder": B_ReminderDate,
            "email_reminder_date": ch_ReminderDate,
            "email": R_email,
            "concept":B_WishConcept,
            "important": B_Important,
            "date":B_Date
        };
        
        addReminder(storeJSON);

        console.log(sendingJSON);
        const options = {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
                }, 
            body: JSON.stringify(sendingJSON)
        }
        fetch('/api',options);
        $( "#saved" ).text( "Birthday Added..." ).show().fadeOut( 5000 );
        closeModul();
    } 
}

const reminderNotif = document.getElementById("s_remModal");

showTodaysReminder();

function sendReminder(){
    if (document.forms["remForm"]["s_remText"].value !== "")
{
    var B_Name =  $('#s_Name').text();
    var B_ReminderDate =  $('#s_remDate').text();
    var B_Concept =  $('#s_remText').val();
    var storeJSON = {
        "name": B_Name,
        "reminder": B_ReminderDate,
        "email_reminder_date": "",
        "email": "",
        "concept":B_Concept,
        "important": "",
        "date":B_ReminderDate
    };
    
    addReminder(storeJSON);
    $( "#saved" ).text( "Concept Saved..." ).show().fadeOut( 5000 );
} 
};



function showTodaysReminder(){
    console.log("SHOW REMINDER")
    var rem = getTodaysReminder();
    console.log(rem);
    rem.forEach(element => {
        document.getElementById("s_Name").innerText = element.name;
        document.getElementById("s_remDate").innerText = element.month +'/'+element.day+'/'+element.year;
        document.getElementById("s_remText").innerText = element.koncept;
        modalbkgrnd.style.display ="block";
        todayNotif.style.display = "block";

    });
   
}

showTodaysB();

function showTodaysB(){
    var rem = getTodaysB();
    rem.forEach(element => {
        document.getElementById("s_tName").innerText = element.name;
        document.getElementById("s_tDate").innerText = element.month +'/'+element.day+'/'+element.year;
        document.getElementById("s_tText").innerText = element.koncept;
      
        todaybkgrnd.style.display ="block";
        reminderNotif.style.display = "block";

    });
   
}

function clearBirthdayModel(){
    document.forms["postForm"]["B_Name"].value = "";
    document.forms["postForm"]["B_Date"].value ="";
    $('#postForm .error')[1].innerText = "";
    $('#postForm .error')[0].innerText = "";
}



//sets in add new birthday default value
const calen = document.getElementById("calendar");
const sidebar = document.getElementById("show-birthdays");
 

/**
 * Shows calendars for current month
 */
function renderCalendar(){
    renderMonth(months[dt.getMonth()],dt.getFullYear());

    const month = dt.getMonth();
    const year = dt.getFullYear();

    const firstDayOfMonth = new Date(year, month, 1);
    const lastDayMonth = new Date(year, month + 1, 0).getDate();

    const startDays = firstDayOfMonth.getDay();// the day in the week the month starts
    sidebar.innerHTML = "";
    calen.innerText="";

    let i = 1;
    for( let x = startDays; x>0;x--){
        const oneDay = document.createElement('div');
        oneDay.classList.add('prevMonthDay');
        calen.appendChild(oneDay);

    }

    for( i; i<=lastDayMonth;i++){
        const oneDay = document.createElement('div');
        oneDay.classList.add('day');
        oneDay.innerText = i;

        const DayBirthdays = document.createElement('div');
        DayBirthdays.classList.add('eventsinDay');
        oneDay.appendChild( DayBirthdays);   
        
        if (i === currDate.getDate() && month === currDate.getMonth()) {
            oneDay.classList.add('currentDay');
        }
       
        calen.appendChild(oneDay);   
        
        
        
    }

    for(i;(i + startDays - 1)%7 !== 0;i++){
        const oneDay = document.createElement('div');
        oneDay.classList.add('nextMonthDay');
        calen.appendChild(oneDay);  
    }
}




/**
 * Shows modul of reminder input
 */
function reminderShow(){
    var x = document.getElementById("B_Name").value;
   

    document.getElementById("nametoreminder").innerHTML=x;
   
    var y = document.getElementById("B_Date").value;
    document.getElementById("r_date").innerHTML= y;
    modalR.style.visibility = "visible";
}

const import_ppl_table =document.getElementById("important_birthdays")
    

function writeImportant(){
    import_ppl_table.innerHTML="";
              
    var important_ppl = getImportant();
    important_ppl.forEach(element =>
        {
            const oneBirthday = document.createElement('div');
    
            oneBirthday.innerText = element.name +': \n' + element.month +'/'+element.day+'/' + element.year;
            import_ppl_table.append(oneBirthday);
                })
}

$( document ).ready(function() {
    //changed which data we vant to view
    $("#isFriendsChecked").click(function() {
        $(".newSidebarBirthday").remove();
        $('.newBirthday').remove();
        renderBirthdays(dt.getMonth()+1);
        rendersideBirthdays(dt.getDate());
    });

    //changed which data we want to view
    $("#isFamilyChecked").click(function() {
        $(".newSidebarBirthday").remove();
        $('.newBirthday').remove();
        renderBirthdays(dt.getMonth()+1);
        rendersideBirthdays(dt.getDate());
    });

        
    $("#prev").click( function(){
        dt.setMonth(dt.getMonth()-1);
        renderCalendar();
        renderBirthdays(dt.getMonth()+1);
        rendersideBirthdays(dt.getDate());
    });

    $("#next").click( function(){
        dt.setMonth(dt.getMonth()+1);
        renderCalendar();
        renderBirthdays(dt.getMonth()+1);
        rendersideBirthdays(dt.getDate());
    });

   
    $("#calendar").on("click", "div.day", function(){
        var oneDay = $(this).text();
        console.log(oneDay);
        dt.setDate(oneDay);
        rendersideBirthdays(oneDay)
    });

   

    $('#postForm').submit(function(e){
        e.preventDefault(); 
        submittingForm();
    });


    //loads calendar
    renderCalendar();


    /**
     * Show data for today at the side
     */
    rendersideBirthdays(dt.getDate());


    writeImportant();

});



//updates view
setInterval(renderBirthdays(dt.getMonth()+1), 3000);


/**
 * Shows data inside of calendar each day and the month we are currently viewing
 * @param {number} month 
 */
function renderBirthdays(month){
    var JSONtoSend= {
        date: dt,
        Family: true,
        Friends: true,
        Work: true,
        getFullMonth: true
    }

    const options = {
        method:"GET",
    }
    if ($("#isFamilyChecked").is(":checked")) {
        url = '/get/'+ month  + '/Family';
        console.log(url);
        fetch(url,options)
            .then( function(response){
                return response.json();
            })
            .then(function (data){
                birthdays = (data);

                birthdays.forEach(element => {
                      $(".day").each(function(index){
                        
                        var d = document.getElementsByClassName("day")[index];
                        if(d.innerText==element.day){

                            const oneBirthday = document.createElement('div');
                            oneBirthday.classList.add("newBirthday");
                            oneBirthday.style.background = colorFamily;
                            d.append(oneBirthday);
                        }
                });
                });
            
              
                

            });
    } 
    if ($("#isFriendsChecked").is(":checked")) {
        url = '/get/'+ (dt.getMonth()+1)  + '/Friends';
        console.log(url);
        fetch(url,options)
            .then( function(response){
                return response.json();
            })
            .then(function (data){
                
                birthdays = (data);

                birthdays.forEach(element => {
                    $(".day").each(function(index){
                      
                      var d = document.getElementsByClassName("day")[index];
                      if(d.innerText==element.day){

                          const oneBirthday = document.createElement('div');
                          oneBirthday.classList.add("newBirthday");

                          oneBirthday.style.background = colorFriends;
                          d.append(oneBirthday);
                      }
                });
                });
            });
    }
    
};

/**
 * Shows data at the side of apaplication for given day and the month we are currently viewing
 * @param {number} day 
 */
function rendersideBirthdays(day){
    document.getElementById("ClickedDate").innerText='Birthdays on: \n' + (dt.getMonth()+1) +'/'+dt.getDate()+'/' + dt.getFullYear();
       
    sidebar.innerHTML="";
    var month = dt.getMonth();
    // $( "#loading" ).text( "Loading birthdays from server" ).show();
    var JSONtoSend= {
        date: dt,
        Family: true,
        Friends: true,
        Work: true,
        getFullMonth: true
    }

    const options = {
        method:"GET",
    }

    if ($("#isFamilyChecked").is(":checked")) {
        url = '/get-day/' +day+ '/'+ (dt.getMonth()+1)  + '/Family';
        fetch(url,options)
            .then( function(response){
                return response.json();
            })
            .then(function (data){
                birthdays = (data);
                
                birthdays.forEach(element => {
                    
                    const sidebarBirthD = document.createElement('div');
                    sidebarBirthD.innerText = element.name +" (" + (dt.getFullYear() - element.year) + ")"; 
                    sidebarBirthD.classList.add("newSidebarBirthday");
                    sidebar.append(sidebarBirthD);
                    sidebarBirthD.style.background = colorFamily;
                        
                
                });
            
              
                

            });
    } 
    if ($("#isFriendsChecked").is(":checked")) {
        url = '/get-day/' +day+ '/'+ (dt.getMonth()+1)  + '/Friends';
        console.log(url);
        fetch(url,options)
            .then( function(response){
                return response.json();
            })
            .then(function (data){
                birthdays = (data);
                birthdays.forEach(element => {
                    
                    const sidebarBirthD = document.createElement('div');
                    sidebarBirthD.innerText = element.name +" (" + (dt.getFullYear() - element.year) + ")"; 
                    sidebarBirthD.classList.add("newSidebarBirthday");
                    sidebar.append(sidebarBirthD);
                    sidebarBirthD.style.background = colorFriends;
               
                
                });

            });
    }
    
};




