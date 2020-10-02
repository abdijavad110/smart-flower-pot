
const minTemp = 10;
const maxTemp = 50;

const minHumidity = 5;
const maxHumidity = 95;

const minBrightness = 1000;
const maxBrightness = 5000;

$(function() {
    let onoff_btn = $('#onoff_button');
    onoff_btn.on('click', function () {
        if (onoff_btn.hasClass('btn-outline-success')){
            onoff_btn.removeClass('btn-outline-success');
            onoff_btn.addClass('btn-outline-danger');
            onoff_btn.text('غیرفعال');
        } else {
            onoff_btn.removeClass('btn-outline-danger');
            onoff_btn.addClass('btn-outline-success');
            onoff_btn.text('فعال');
        }
    });
    update();
    setInterval(update, 7000);
});


function update() {
    let settings = {}

    $(".custom-range").each(function () {
        let str = $(this).attr('id');
        if (!str){return;}
        str = str.toString();
        let id = str[str.length-1];
        let command = str.substr(0, str.length-1);
        if (!settings[id]){
            settings[id] = {ip: $(`#ip${id}`).text()    }
        }
        if (command == 'setTemperature'){
            settings[id][command] = ($(this).val());
        } else if (command == 'setHumidity'){
            settings[id][command] = ($(this).val());
        } else if (command == 'setBrightness') {
            settings[id][command] = ($(this).val());
        }
    });


    $.post('/client',
        {agents: JSON.stringify(settings)},
        function (data, status) {
            if (data.length == 0 || !data){
                $('#agent_list').html(`
                <div class="card border-primary mb-3" style="max-width: 100%; height: 25%; text-align: center;">
                    <div class="card-header"></div>
                    <div class="card-body">
                      <h4 class="card-title">هیچ دستگاهی به سامانه متصل نشده است!</h4>
                      <p class="card-text">لطفا دستگاه‌های مورد نظر را راه‌اندازی و به شبکه متصل کنید، سپس کمی منتظر بمانید تا ارتباط بین آن‌ها و سرور برقرار شود.</p>
                    </div>
                  </div>
`               );
                return;
            }
            $('#agent_list').html('');
            for (agent in data) {
                addAgent(data[agent], agent);
            }
            $('.light-control').on('mousemove  change', function () {
                let val = ($(this).val());
                let str =  `  ${toFaDigit(val)} لوکس  `;
                $(`#${$(this).attr('for')}`).text(str);
            });

            $('.hum-control').on('mousemove change', function () {
                let val = ($(this).val());
                let str =  `${toFaDigit(val)} درصد  `;
                $(`#${$(this).attr('for')}`).text(str);
            });
        }
    )
}


let digits = ['۰', '۱', '۲', '۳', '۴', '۵', '۶', '۷', '۸', '۹'];

function toFaDigit(num) {
    num = Math.floor(num).toString();
    let res = '';
    for (let i = 0 ; i < num.length ;i ++)
        res+= digits[num[i]];
    return res;
}

function addAgent(agent, id) {
    console.log(agent.setBrightness);
    let agentsList = $("#agent_list");
    agentsList.append(`
          <div class="col-md-5 bg-dark rounded p-3 m-3" style="height: 40%;">
            <h5 class="text-light" style="text-align: center">${agent.name}</h5>
            <hr class="w-100 mt-2 mb-3" style="border-top: 1px solid var(--light);">
            <div class="row mx-1 mb-3">
              <div class="col-md-3 text-center">
                <h6 class="text-muted">وضعیت</h6>
              </div>
              <div class="col-md-9 text-center">
                <h6 class="text-muted">تنظیم</h6>
              </div>
            </div>
            <div class="row mx-1">
              <div class="col-md-4 text-center">
                <i class="fa fa-thermometer-three-quarters fa-2x mb-3 text-danger"> ${(agent.temperature)} </i>
              </div>
              <div class="col-md-5 text-center" style="vertical-align: middle">
                <input type="range" id="setTemperature${id}" class="custom-range min='${minTemp}' max='${maxTemp}' temp-control" value='${(agent.setTemperature)}' disabled=""  dir="ltr" for="temperature${id}-indicator">
              </div>
              <div class="col-md-3 text-right" style="vertical-align: middle">
                <label class="label text-muted value-indicator" dir="rtl"  id="temperature${id}-indicator">----</label>
              </div>
            </div>
            <div class="row mx-1">
              <div class="col-md-4 text-center">
                <i class="fa fa-tint fa-2x mb-3 text-info"> ${agent.humidity} </i>
              </div>
              <div class="col-md-5 text-center" style="vertical-align: middle">
                <input type="range" id="setHumidity${id}" class="custom-range hum-control" min='${minHumidity}' max='${maxHumidity}' value='${(agent.setHumidity)}' dir="ltr" for="humidity${id}-indicator">
              </div>
              <div class="col-md-3 text-right" style="vertical-align: middle">
                <label class="label text-muted value-indicator" dir="rtl"  id="humidity${id}-indicator">${toFaDigit(agent.setHumidity)} درصد</label>
              </div>
            </div>
            <div class="row mx-1">
              <div class="col-md-4 text-center">
                <i class="fa fa-sun fa-2x mb-3" style="color: var(--sunshine)"> ${(agent.brightness)}</i>
              </div>
              <div class="col-md-5 text-center" style="vertical-align: middle">
                <input type="range" id="setBrightness${id}" class="custom-range light-control" min='${minBrightness}' max='${maxBrightness}' value='${(agent.setBrightness)}'  dir="ltr" for="brightness${id}-indicator">
              </div>
              <div class="col-md-3 text-right" style="vertical-align: middle">
                <label class="label text-muted value-indicator" dir="rtl"  id="brightness${id}-indicator">
                    ${toFaDigit(agent.setBrightness)} لوکس 
                  </label> 
              </div>
            </div>
          </div>
          <div style="display: none" id="ip${id}">${agent.ip}</div>
    `);
}
