var DAYS = 3;
var ROWS = 20;
var COLWIDTH = 14.25;
var delIndex = -1;
//                    YYYY  MM  DD  HH  MM  
// const d = new Date(2018, 11, 24, 10, 33);

function make_zero(inp){
  if (inp < 10){
    return "0"+inp;
  } else {
    return inp.toString()
  }
}

function check_table(arr, row, hour, min, day){
  var timestampRef = +new Date(`2000-01-${make_zero(day)}T${make_zero(hour)}:${make_zero(min)}:00Z`);

  for (let i = 0; i<arr.length; i++){
    var timestampStart = +new Date(`2000-01-${make_zero(arr[i][1][2])}T${make_zero(arr[i][1][0])}:${make_zero(arr[i][1][1])}:00Z`);
    var timestampStop = +new Date(`2000-01-${make_zero(arr[i][2][2])}T${make_zero(arr[i][2][0])}:${make_zero(arr[i][2][1])}:00Z`);

    if(arr[i][0] == row && timestampRef >= timestampStart &&
                           timestampRef < timestampStart + 900000){ // 15 minutes in ms
      
      console.log("Got it");
      return [arr[i], Math.round((timestampStop-timestampStart)/900000)];
    } else if(arr[i][0] == row && timestampRef >= timestampStart &&
                                  timestampRef < timestampStop){ // If its not the first 15 minutes½
      return [arr[i], 0];
    }
  }
  return [-1, -1];
}

function exportToJsonFile(jsonData, fileName) {
    let dataStr = JSON.stringify(jsonData);
    let dataUri = 'data:application/json;charset=utf-8,'+ encodeURIComponent(dataStr);

    let exportFileDefaultName = `${fileName}.rye`;

    let linkElement = document.createElement('a');
    linkElement.setAttribute('href', dataUri);
    linkElement.setAttribute('download', exportFileDefaultName);
    linkElement.click();
}

var people = [];
var currentPeople = 0;

////////////////////////////////////////////////////////////////////////////////
//
//    CANVAS CONTROLLER
//
////////////////////////////////////////////////////////////////////////////////

function get_max(arr){
  maxNum = arr[0];
  for(let i = 0; i < arr.length; i++){
    if(maxNum < arr[i]){
      maxNum = arr[i]
    }
  }
  return maxNum;
}


const canvas = document.getElementById("myCanvas");
const ctx = canvas.getContext("2d");

function draw_canvas(){
  canvas.width  = COLWIDTH * people.length;
  canvas.height = get_max(people) * 10 + 10
  
  // Line diagram
  ctx.beginPath();
  ctx.lineWidth = 1;
  ctx.strokeStyle = "#d0d0d0";

  for(let i = 0; i < get_max(people) + 1; i++){
    console.log(i)
    ctx.moveTo(0, i*10+5);
    ctx.lineTo(canvas.width, i*10+5);
    ctx.stroke();
  }

  ctx.closePath()
  ctx.beginPath()

  ctx.lineWidth = 3;
  ctx.strokeStyle = "#FF0000";
  ctx.moveTo(0, canvas.height - 5)

  for (let i = 0; i < people.length; i++){
    var height = people[i] * -10
    // Block diagram
    // ctx.fillRect(i * COLWIDTH, canvas.height, COLWIDTH+1, height);
  
    // Line diagram
    ctx.lineTo(i * COLWIDTH, height + canvas.height - 5);
  }

  // Line diagram
  ctx.stroke();
}

draw_canvas()


////////////////////////////////////////////////////////////////////////////////
//
//    TABLE DRAWING
//
////////////////////////////////////////////////////////////////////////////////

function create_table(data){
  people = [];
  currentPeople = 0;
  //                           DD HH MM 
  var time = new Date("2000-01-01T00:00:00Z");

  if (document.getElementsByClassName("del1").length != 0) {
    document.getElementsByClassName("del1")[0].remove();
  } 

  // CREATE TABLE
  let table = document.createElement('table');
  let thead = document.createElement('thead');
  let tbody = document.createElement('tbody');

  table.classList.add('del1');

  table.appendChild(thead);
  table.appendChild(tbody);

  // Adding the entire table to the body tag
  document.getElementsByClassName("table")[0].appendChild(table);

  var tableRows = [];
  for(let i = 0; i < ROWS; i++){
    tableRows.push(document.createElement('tr'));
  }
  console.log(tableRows)

  let row_1 = document.createElement('tr'); // Create header row

  while (time.getDate() < DAYS + 1){
    if (time.getMinutes() == 0){
      console.log("CREATED HEADER")
      let heading_1 = document.createElement('th');
      heading_1.innerHTML = `${make_zero(time.getHours())}:${make_zero(time.getMinutes())}`;
      heading_1.colSpan = "4"
      row_1.appendChild(heading_1);
    }

    // console.log(time.getDate(), time.getHours(), time.getMinutes(), `${time.getHours()}:${time.getMinutes()}`);
    
    // Add data rows
    for(let i = 0; i < ROWS; i++){
      row = tableRows[i];
      rowData = document.createElement('td');
      rowData.classList.add('clickable');

      var response = check_table(data, i, time.getHours(), time.getMinutes(), time.getDate())
      //console.log(response + `${time.getHours()}:${time.getMinutes()}`)
      if (response[1] != -1){
          // console.log(response, currentPeople)
          currentPeople += response[0][4];
        if(response[1] != 0){
          rowData.style.backgroundColor = "#71bde3";
          rowData.colSpan = response[1].toString();
          rowData.style.maxWidth = `${response[1]*10}px`;
          rowData.addEventListener("click", view_time.bind(null, response[0]));
          rowData.style.paddingLeft = "5px";
          
          rowData.innerHTML = response[0][5]//.slice(0, response[1])
          
        }
      } else {
        if (time.getHours() % 2 == 0){
          rowData.classList.add('grayed');
        }

        rowData.addEventListener("click", add_time.bind(null, time.getHours(), time.getMinutes(), time.getDate(), i));
        rowData.innerHTML = ''
      }
      
      if (response[1] != 0){
        row.appendChild(rowData)
        rowData.style.minWidth = `10px`;
      }
    }

    people.push(currentPeople);
    currentPeople = 0;

    time.setMinutes(time.getMinutes() + 15)
  }

  thead.appendChild(row_1);

  for(let i = 0; i < ROWS; i++){
    tbody.appendChild(tableRows[i])
  }

console.log(people)
draw_canvas()
}

var tableArr = [
// ROWID, [HOUR, MIN, DAY], [hour, min, day], VAGTER, ANTAL, MØDESTED
//  [2, [11, 30, 1], [14, 0, 1], 3, 2, "Lige her hvor der er at jeg gemmer mgi lige nu"],
//  [1, [13, 30, 1], [15, 0, 1], 2, 1, "I vim hullet :o"],
]

create_table(tableArr) // Create the table and insert it on the page

////////////////////////////////////////////////////////////////////////////////
//
//    FORM CONTROLLERS
//
////////////////////////////////////////////////////////////////////////////////

// Add time triggers when the user click and empty feild in the table, this should create a popup to let the user create a task
function add_time(hours, minutes, day, rowId){
  document.getElementById("sletKnap").style.display = "none"
  document.getElementById("time1").value = `${make_zero(hours)}:${make_zero(minutes)}`;
  document.getElementById("day1").value = `${day}`;

  document.getElementsByClassName("rowholder")[0].innerHTML = rowId

  document.getElementById("createTimeForm").style.display = "block";
}

function stop_form(){
  delIndex = -1;
  document.getElementById("createTimeForm").style.display = "none";
  document.getElementById("time1").value = "";
  document.getElementById("day1").value = "";
  document.getElementById("time2").value = "";
  document.getElementById("day2").value = "";

  document.getElementById("antal").value = "";
  document.getElementById("vagter").value = "";
  document.getElementById("meeting").value = "";

  document.getElementById("sletKnap").style.display = "none"
}

function add_timezone(){
  if (delIndex != -1){
    tableArr.splice(delIndex, 1);
  }

  var time1 = document.getElementById("time1").value.split(":")
  var time2 = document.getElementById("time2").value.split(":")

  if(document.getElementById("time1").value == "" ||
     document.getElementById("time2").value == "" ||
     document.getElementById("day1").value == "" ||
     document.getElementById("day2").value == "" ||
     document.getElementById("vagter").value == "" ||
     document.getElementById("antal").value == "" ||
     document.getElementById("meeting").value == "" 
  ){
    alert("Vær sød at skrive noget i alle felter");
    return -1;
  
  } else if(time1.length != 2 || time2.length != 2){
    alert("Forkert tidsformat");
    return -1;
  
  } 

  var timestampStart = +new Date(`2000-01-${make_zero(parseInt(document.getElementById('day1').value))}T${make_zero(parseInt(time1[0]))}:${make_zero(parseInt(time1[1]))}:00Z`);
  var timestampStop = +new Date(`2000-01-${make_zero(parseInt(document.getElementById('day2').value))}T${make_zero(parseInt(time2[0]))}:${make_zero(parseInt(time2[1]))}:00Z`);
  console.log(timestampStart, timestampStop, "AAAAAAAAAAAAA", `2000-01-${make_zero(parseInt(document.getElementById('day1').value))}T${make_zero(parseInt(time1[0]))}:${make_zero(parseInt(time1[1]))}:00Z`)

  if(timestampStart >= timestampStop){
    alert("Vagten kan ikke have en negativ længte.");
    return -1;
  }

  var myData = [
    parseInt(document.getElementsByClassName("rowholder")[0].innerHTML),
    [parseInt(time1[0]), parseInt(time1[1]), parseInt(document.getElementById("day1").value)],
    [parseInt(time2[0]), parseInt(time2[1]), parseInt(document.getElementById("day2").value)],
    parseInt(document.getElementById("vagter").value),
    parseInt(document.getElementById("antal").value),
    document.getElementById("meeting").value
  ]
  // console.log(myData)

  tableArr.push(myData)
  create_table(tableArr)
  stop_form()
}


// View time triggers when the user clicks an ocupied square, this would allow them to view and edit the feilds
// ROWID, [HOUR, MIN, DAY], [hour, min, day], VAGTER, ANTAL, MØDESTED
function view_time(arr){
  console.log(arr);

  const check_elem = (element) => element == arr;
  delIndex = tableArr.findIndex(check_elem); // If the user desiceds to edit the field we need to know where to edit the values from

  document.getElementById("time1").value = `${make_zero(arr[1][0])}:${make_zero(arr[1][1])}`;
  document.getElementById("day1").value = `${arr[1][2]}`;
  document.getElementById("time2").value = `${make_zero(arr[2][0])}:${make_zero(arr[2][1])}`;
  document.getElementById("day2").value = `${arr[2][2]}`;

  document.getElementById("antal").value = arr[3];
  document.getElementById("vagter").value = arr[4];
  document.getElementById("meeting").value = arr[5];

  document.getElementsByClassName("rowholder")[0].innerHTML = arr[0]
  document.getElementById("sletKnap").style.display = "block"

  document.getElementById("createTimeForm").style.display = "block";
}

function delete_elem(){
  if (delIndex != -1){
    tableArr.splice(delIndex, 1);
  }

  create_table(tableArr)
  stop_form()
}

function export_file(){
  exportToJsonFile(tableArr, "tidsplan");
}

function import_file(){
  document.getElementById("importForm").style.display = "block";
}

async function load_data(){
  const inputEl = document.querySelector("#loadDataFeild");
  const jsonFile = inputEl.files[0];
  const json = await jsonFile.text();
  console.log(json);

  tableArr = JSON.parse(json);
  create_table(tableArr)
  stop_form2()
} 

function stop_form2(){
  document.getElementById("importForm").style.display = "none";
}


////////////////////////////////////////////////////////////////////////////////
//
//    PROTOCOL SWITCH CODE
//
////////////////////////////////////////////////////////////////////////////////
  
function goto_time(){
  document.getElementsByClassName("content")[0].style.display = "block";
  document.getElementsByClassName("people")[0].style.display = "none";
}

function goto_person(){
  document.getElementsByClassName("content")[0].style.display = "none";
  document.getElementsByClassName("people")[0].style.display = "block";

}

//          [[navn, nummer], [navn, nummer]
var personData = [["", ""], ["", ""], ["", ""], ["", ""], ["", ""], ["", ""]];

function create_table2(){
  if (document.getElementsByClassName("del2").length != 0) {
    document.getElementsByClassName("del2")[0].remove();
  } 
  // CREATE TABLE
  let table = document.createElement('table');
  let thead = document.createElement('thead');
  let tbody = document.createElement('tbody');

  table.classList.add('del2');
  
  table.appendChild(thead);
  table.appendChild(tbody);
  
  // Adding the entire table to the body tag
  document.getElementsByClassName("table2")[0].appendChild(table);

  // Create table header
  let header = document.createElement('tr');
  
  let heading_1 = document.createElement('th');
  heading_1.innerHTML = "Navn";

  let heading_2 = document.createElement('th');
  heading_2.innerHTML = "Telefon nummer";

  let heading_3 = document.createElement('th');
  heading_3.innerHTML = "Action";

  header.appendChild(heading_1);
  header.appendChild(heading_2);
  header.appendChild(heading_3);
  thead.appendChild(header);

  for(let i = 0; i < personData.length; i++){
    // Create the row
    let row = document.createElement("tr");

    // Add name as item
    let rowData = document.createElement("td");
    let input = document.createElement("input"); 
    input.value = personData[i][0];
    input.classList.add(`name-${i}`);
    rowData.appendChild(input);

    // Add phone number as item
    let rowData2 = document.createElement("td");
    let input2 = document.createElement("input"); 
    input2.value = personData[i][1].toString();
    input2.classList.add(`num-${i}`);
    rowData2.appendChild(input2);

    // Allow for deletion of row
    let rowData3 = document.createElement("td");
    let input3 = document.createElement("button"); 
    
    input3.innerHTML = "Slet Række";
    input3.addEventListener("click", del_row.bind(null, i));
    
    rowData3.appendChild(input3);

    row.appendChild(rowData);
    row.appendChild(rowData2);
    row.appendChild(rowData3);

    tbody.appendChild(row);
  }

  // Create last row where you can add more rows :0
  let row = document.createElement("tr");

  let rowData = document.createElement("td");
  rowData.colSpan = "3";

  let input = document.createElement("button"); 
  input.innerHTML = "Tilføj række";
  input.addEventListener("click", add_row.bind(null));

  rowData.appendChild(input);
  row.appendChild(rowData);
  tbody.appendChild(row);
  
}

create_table2();

function get_table_contents(){
  var personDataTemp = [];
  for(let i = 0; i < personData.length; i++){
    var nam = document.getElementsByClassName(`name-${i}`)[0].value;
    var num = document.getElementsByClassName(`num-${i}`)[0].value;
    personDataTemp.push([nam, num]);
  }

  personData = personDataTemp;
  return personDataTemp;
}

function add_row(){
  get_table_contents();
  personData.push(["", ""]);
  create_table2();
}

function del_row(indx){
  get_table_contents();
  personData.splice(indx, 1);
  create_table2();
}

function export_person_file(){
  get_table_contents();
  exportToJsonFile(personData, "persondata");
}

function import_file2(){
  document.getElementById("importForm2").style.display = "block";
}
function stop_form3(){
  document.getElementById("importForm2").style.display = "none";
}

async function load_person_data(){
  const inputEl = document.querySelector("#loadDataFeild2");
  const jsonFile = inputEl.files[0];
  const json = await jsonFile.text();
  console.log(json);

  personData = JSON.parse(json);
  create_table2()
  stop_form3()
} 

