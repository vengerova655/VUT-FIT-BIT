/**
 * @author: Veronika Vengerova(xvenge01)
 */

const express = require('express');
const app = express();
app.listen(5000, ()=> console.log("listening at 5000"));
app.use(express.static('public'));
app.use(express.json());
const fs = require('fs');

/**
 * Function for writing data from server to client
 */
app.post('/api', (request, response)=>{

    console.log('I got a request');
    console.log(request.body);

    fs.readFile('birthdays.json', 'utf8', (err,data) => {
        if(err){
            console.error(err);
            return
        }
            let birthd =[];
            birthd = JSON.parse(data);
            var str = JSON.stringify(request.body.date);
            
            console.log(str);
            var date = str.split('-');

           
            var toSave = {
                name : request.body.name,
                day : date[2].split('"')[0],
                year : date[0].split('"')[1],
                month : date[1],
                category: request.body.category
            }
            birthd.push(toSave);
            birthdayjson = JSON.stringify(birthd);
            console.log('adding to file');
            fs.writeFile('birthdays.json',birthdayjson,function(err){
                if(err) return console.log(err);
                console.log('added to file');
            });
    });
});




/**
 * Functions for getting data from json file birthday.json
 */

app.get('/get-day/:day/:month/:category', (req,res) =>{
    fs.readFile('birthdays.json', 'utf8', (err,data) => {
        if(err){
            console.error(err);
            return
        }
        let wanted = [];
        let x =JSON.parse(data);
        x.forEach(element => {
            if((element.month) == req.params.month && element.day == req.params.day && element.category == req.params.category){
                 wanted.push(element);
            }
        });
            res.send(JSON.stringify(wanted)); 
             
    });
        
    console.log("get-data called");
    
});




app.get('/get/:month/:category', (req,res) =>{

    fs.readFile('birthdays.json', 'utf8', (err,data) => {
        if(err){
            console.error(err);
            return
        }
        let wanted = [];
        let x =JSON.parse(data);
        x.forEach(element => {     
            if((element.month) === req.params.month && element.category === req.params.category){
                 wanted.push(element);
            }
        });
            console.log(wanted);
            res.send(JSON.stringify(wanted)); 
             
    });
        
    console.log("get-data called");
    
});
