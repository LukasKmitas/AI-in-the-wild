// Dr Oisin Cawley SETU 2024
// This version is mostly a copy from the Google Tiny Sorter Example: 
// https://experiments.withgoogle.com/tiny-sorter/view
// However their version of serial.js did not work for me since it was harcoding the device interfaces.
// A better version was availbale at https://webusb.github.io/arduino/demos/serial.js
// Here they iterate through the interfaces to find the correct one.
// That piece of iteration and the related modifications have been updated into this version.

(function() {
  'use strict';

  document.addEventListener('DOMNodeInserted', event => {
    let connectButton = document.querySelector("#connect");
    


    function connect() {
      port.connect().then(() => {
        
        connectButton.textContent = 'DISCONNECT';

        port.onReceive = data => {
          let textDecoder = new TextDecoder();
          console.log(textDecoder.decode(data));
        }
        port.onReceiveError = error => {
          console.error(error);
        };
      }, error => {
        
      });
    }
    try {
    connectButton.addEventListener('click', function() {
      if (port) {
        port.disconnect();
        connectButton.textContent = 'CONNECT ARDUINO';
        
        port = null;
      } else {
        serial.requestPort().then(selectedPort => {
          port = selectedPort;
          connect();
        }).catch(error => {
          
        });
      }
    });
      
    }
    catch (e) {
      
      console.log("p5 sketch not loaded yet: ", e);
    }

//     serial.getPorts().then(ports => {
//       if (ports.length == 0) {
        
//       } else {
        
//         port = ports[0];
//         connect();
//       }
//     });
  });
})();


// From https://github.com/webusb/arduino/blob/gh-pages/demos/serial.js
var serial = {};

(function() {
  'use strict';

  serial.getPorts = function() {
    return navigator.usb.getDevices().then(devices => {
      console.log(devices);
      return devices.map(device => new serial.Port(device));
    });
  };

  serial.requestPort = function() {
    const filters = [
      { 'vendorId': 0x2341, 'productId': 0x8036 },
      { 'vendorId': 0x2341, 'productId': 0x8037 },
      { 'vendorId': 0x2341, 'productId': 0x804d },
      { 'vendorId': 0x2341, 'productId': 0x804e },
      { 'vendorId': 0x2341, 'productId': 0x804f },
      { 'vendorId': 0x2341, 'productId': 0x8050 },
    ];
    return navigator.usb.requestDevice({ 'filters': filters }).then(
      device => new serial.Port(device)
    );
  }

  serial.Port = function(device) {
    this.device_ = device;
	//OC Edit here
    this.interfaceNumber_ = 2;  // original interface number of WebUSB Arduino demo
    this.endpointIn_ = 5;       // original in endpoint ID of WebUSB Arduino demo
    this.endpointOut_ = 4;      // original out endpoint ID of WebUSB Arduino demo
  };

  serial.Port.prototype.connect = function() {
    let readLoop = () => {
      this.device_.transferIn(5, 64).then(result => {
        this.onReceive(result.data);
        readLoop();
      }, error => {
        this.onReceiveError(error);
      });
    };

	//OC Edit below
    return this.device_.open()
        .then(() => {
          if (this.device_.configuration === null) {
            return this.device_.selectConfiguration(1);
          }
        })
.then(() => {
          var configurationInterfaces = this.device_.configuration.interfaces;
          configurationInterfaces.forEach(element => {
            element.alternates.forEach(elementalt => {
              if (elementalt.interfaceClass==0xff) {
                this.interfaceNumber_ = element.interfaceNumber;
                elementalt.endpoints.forEach(elementendpoint => {
                  if (elementendpoint.direction == "out") {
                    this.endpointOut_ = elementendpoint.endpointNumber;
                  }
                  if (elementendpoint.direction=="in") {
                    this.endpointIn_ =elementendpoint.endpointNumber;
                  }
                })
              }
            })
          })
        })
        .then(() => this.device_.claimInterface(this.interfaceNumber_))
        .then(() => this.device_.selectAlternateInterface(this.interfaceNumber_, 0))
        // The vendor-specific interface provided by a device using this
        // Arduino library is a copy of the normal Arduino USB CDC-ACM
        // interface implementation and so reuses some requests defined by
        // that specification. This request sets the DTR (data terminal
        // ready) signal high to indicate to the device that the host is
        // ready to send and receive data.
        .then(() => this.device_.controlTransferOut({
            'requestType': 'class',
            'recipient': 'interface',
            'request': 0x22,
            'value': 0x01,
            'index': this.interfaceNumber_}))
        .then(() => {
          readLoop();
        });
  };
  serial.Port.prototype.disconnect = function() {
    return this.device_.controlTransferOut({
            'requestType': 'class',
            'recipient': 'interface',
            'request': 0x22,
            'value': 0x00,
            'index': this.interfaceNumber_}) //OC Edit here
        .then(() => this.device_.close());
  };

  serial.Port.prototype.send = function(data) {
    return this.device_.transferOut(this.endpointOut_, data); //OC Edit here
  };
  serial.Port.prototype.receive = function(data) {
    return this.device_.transferIn(this.endpointIn_, data); //OC Edit here
  };

})();

