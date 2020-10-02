const mongoose = require('mongoose');



const Agent = mongoose.model('Agent',
    {
            ip: {type: String, required: true},
            name: {type: String, default: "گیاه جدید"},
            state: {type: Boolean, default: true},

            temperature: {type: Number, default: 25},
            humidity: {type: Number, default: 70},
            brightness: {type: Number, default: 3500},

            setTemperature: {type: Number, default: 25},
            setHumidity: {type: Number, default: 70},
            setBrightness: {type: Number, default: 3500},
    });


module.exports = Agent;
