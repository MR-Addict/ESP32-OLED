//chart temperature
var chartT = new Highcharts.Chart({
  chart: { renderTo: "chart-temperature" },
  title: { text: "Temperature" },
  series: [
    {
      showInLegend: false,
      data: [],
    },
  ],
  plotOptions: {
    line: { animation: false, dataLabels: { enabled: true } },
    series: { color: "#059e8a" },
  },
  xAxis: { categories: [] },
  yAxis: {
    title: { text: "Temperature (°C)" },
  },
  credits: { enabled: false },
});
//chart pressure
var chartP = new Highcharts.Chart({
  chart: { renderTo: "chart-pressure" },
  title: { text: "Pressure" },
  series: [
    {
      showInLegend: false,
      data: [],
    },
  ],
  plotOptions: {
    line: { animation: false, dataLabels: { enabled: true } },
    series: { color: "#18009c" },
  },
  xAxis: { categories: [] },
  yAxis: {
    title: { text: "Pressure (Pa)" },
  },
  credits: { enabled: false },
});
//websocket
var websocket = new WebSocket("ws://" + location.hostname + ":81/");
websocket.onopen = function (event) {
  onOpen(event);
};
websocket.onclose = function (event) {
  onClose(event);
};
websocket.onmessage = function (event) {
  onMessage(event);
};
websocket.onerror = function (event) {
  onError(event);
};
function onOpen(event) {
  console.log("Server Connected!");
  var today = new Date();
  var MYJSON =
    '{"Year":' +
    today.getFullYear() +
    ',"Month":' +
    (today.getMonth() + 1) +
    ',"Date":' +
    today.getDate() +
    ',"Hour":' +
    today.getHours() +
    ',"Minute":' +
    today.getMinutes() +
    ',"Second":' +
    today.getSeconds() +
    "}";
  websocket.send(MYJSON);
}
function onClose(event) {
  console.log("Server Disconnected!");
  alert("Server Disconnected!");
}
function onMessage(event) {
  var JSONContent = JSON.parse(event.data);
  if (JSONContent.hasOwnProperty("time")) var x = JSONContent.time;
  if (JSONContent.hasOwnProperty("temperature")) {
    var y = JSONContent.temperature;
    chartT.xAxis[0].categories.push(x);
    chartT.update({ xAxis: { categories: chartT.xAxis[0].categories } });
    if (chartT.series[0].data.length > 24) {
      chartT.series[0].addPoint([x, y], true, true, true);
    } else {
      chartT.series[0].addPoint([x, y], true, false, true);
    }
  }
  if (JSONContent.hasOwnProperty("pressure")) {
    y = JSONContent.pressure;
    chartP.xAxis[0].categories.push(x);
    chartP.update({ xAxis: { categories: chartP.xAxis[0].categories } });
    if (chartT.series[0].data.length > 24) {
      chartP.series[0].addPoint([x, y], true, true, true);
    } else {
      chartP.series[0].addPoint([x, y], true, false, true);
    }
  }
}
function onError(event) {
  console.log("Error:" + event.data);
  alert("Error Occured!");
}
