/*
 * Function to report a too-dry plant from a Helium Atom and Element combo.
 *
 * In Configuration, set:
 * - YOUR_PHONE_NUMBER : E.164 formatted number to text
 * - TWILIO_NUMBER : E.164 formatted number from Twilio with SMS capabilities
 * - SECRET_API_KEY : 'top-secret' by default, or change the code.
 *
 * Below, change:
 * - TOO_DRY : Treshold to warn about a too dry plant.
 */
var TOO_DRY = 30; 


// Validate incoming passcode from the Helium
function auth(code) {
  return code == process.env.SECRET_API_KEY;
}


// Handle moisture readings
exports.handler = function(context, event, callback) {
    if (!isNaN(event.percent) && auth(event.passcode)) {
      if (parseFloat(event.percent) < TOO_DRY) {
        let msg = `Water your plant! It's reading only ${event.percent}% humidity.`;
        var client = context.getTwilioClient();
        return client.messages.create({
          to: context.YOUR_PHONE_NUMBER, 
          from: context.TWILIO_NUMBER, 
          body: msg
        }).then(() => {
          callback(null, {result: 'Message sent.'});
        });
      } else {
        callback(null, {result: 'Enough moisture.'});
      }
    } else {
        callback(null, {result: 'Message unsent.'});
    }
};