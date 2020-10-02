const express = require('express');
const router = express.Router();
const mongoose = require('mongoose');
const Agent = require('../models/agent');

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get('/persp', function(req, res, next) {
    // res.render('tmp', { title: 'Express' });
});

let default_response = {
    temperature: 25,
    humidity: 70,
    brightness: 3500,
};

// agent connection
router.post('/agent', function (req, res, next) {
    let ip = req.connection.remoteAddress;

    Agent.findOne({ip: ip}, function (err, agent) {
        if (err) {
            res.send("An error has happened!");
        }
        console.log(agent, ip);
        if (agent) {
            console.log(res.body);
            res.send(agent);
        } else {
            let agent = new Agent({
                ip: ip,
            });
            agent.save().then(() => res.send(agent));
        }
        try {
            agent.brightness = req.body.brightness;
            agent.temperature = req.body.temperature;
            agent.humidity = req.body.humidity;
            agent.save();
            console.log(agent);
        } catch (e) {
            console.warn(e);
        }
    });
});


async function updateAgents(agents) {
    for (agent in agents) {
        // console.log(agents[agent].setTemperature);
        // console.log(agents[agent].setHumidity);
        // console.log(agents[agent].setBrightness);
        await Agent.updateOne({ip: agents[agent].ip},
            {
                $set: {
                    setTemperature: agents[agent].setTemperature,
                    setHumidity: agents[agent].setHumidity,
                    setBrightness: agents[agent].setBrightness,
                    Temperature: agents[agent].Temperature,
                    Humidity: agents[agent].Humidity,
                    Brightness: agents[agent].Brightness,
                }
            });
    }
}

router.post('/client', async function (req, res, next) {
    let agents = JSON.parse(req.body['agents']);
    updateAgents(agents).then(() => {
        // console.warn(agents);
        Agent.find({}, function (err, agents) {
            // console.log(agents)
            res.send(agents);
        });
    });
});


module.exports = router;
