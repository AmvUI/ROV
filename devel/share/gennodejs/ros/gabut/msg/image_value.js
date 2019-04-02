// Auto-generated. Do not edit!

// (in-package gabut.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class image_value {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.state_red = null;
      this.state_blue = null;
      this.blue_crack = null;
    }
    else {
      if (initObj.hasOwnProperty('state_red')) {
        this.state_red = initObj.state_red
      }
      else {
        this.state_red = 0;
      }
      if (initObj.hasOwnProperty('state_blue')) {
        this.state_blue = initObj.state_blue
      }
      else {
        this.state_blue = 0;
      }
      if (initObj.hasOwnProperty('blue_crack')) {
        this.blue_crack = initObj.blue_crack
      }
      else {
        this.blue_crack = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type image_value
    // Serialize message field [state_red]
    bufferOffset = _serializer.int16(obj.state_red, buffer, bufferOffset);
    // Serialize message field [state_blue]
    bufferOffset = _serializer.int16(obj.state_blue, buffer, bufferOffset);
    // Serialize message field [blue_crack]
    bufferOffset = _serializer.int16(obj.blue_crack, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type image_value
    let len;
    let data = new image_value(null);
    // Deserialize message field [state_red]
    data.state_red = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [state_blue]
    data.state_blue = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [blue_crack]
    data.blue_crack = _deserializer.int16(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 6;
  }

  static datatype() {
    // Returns string type for a message object
    return 'gabut/image_value';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'ad9f5988e975c9e2128f2a0d58a5bb28';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int16 state_red
    int16 state_blue
    int16 blue_crack
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new image_value(null);
    if (msg.state_red !== undefined) {
      resolved.state_red = msg.state_red;
    }
    else {
      resolved.state_red = 0
    }

    if (msg.state_blue !== undefined) {
      resolved.state_blue = msg.state_blue;
    }
    else {
      resolved.state_blue = 0
    }

    if (msg.blue_crack !== undefined) {
      resolved.blue_crack = msg.blue_crack;
    }
    else {
      resolved.blue_crack = 0
    }

    return resolved;
    }
};

module.exports = image_value;
