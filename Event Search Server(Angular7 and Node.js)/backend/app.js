const express = require("express");
const https = require("https");
const bodyParser = require("body-parser");

const app = express();

const SpotifyWebApi = require("spotify-web-api-node");
const spotifyApi = new SpotifyWebApi({
  clientId: "0913916072464419d",
  clientSecret: "146f1a2fb",
});

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

// set header
app.use((req, res, next) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader(
    "Access-Control-Allow-Headers",
    "Origin, X-Requested-With, Content-Type, Accept"
  );
  res.setHeader(
    "Access-Control-Allow-Methods",
    "GET, POST, PATCH, DELETE, OPTIONS"
  );
  next();
});

// app.use((req, res) => {
//   res.send('');
//   next();
// });

// searching event
app.get("/searchForm", (req, res, next) => {
  const searchInfo = req.query;

  if (searchInfo.category == "music") {
    searchInfo.category = "KZFzniwnSyZfZ7v7nJ";
  } else if (searchInfo.category == "sports") {
    searchInfo.category = "KZFzniwnSyZfZ7v7nE";
  } else if (searchInfo.category == "at") {
    searchInfo.category = "KZFzniwnSyZfZ7v7na";
  } else if (searchInfo.category == "film") {
    searchInfo.category = "KZFzniwnSyZfZ7v7nn";
  } else if (searchInfo.category == "spomiscellaneousrts") {
    searchInfo.category = "KZFzniwnSyZfZ7v7n1";
  } else {
    searchInfo.category = "";
  }

  var geohash = require("ngeohash");
  
  // send request
  if (typeof searchInfo.geoCode === "object") {
    searchInfo.geoHash = geohash.encode(
      searchInfo.geoCode[0],
      searchInfo.geoCode[1],
      5
    );
    let myUrl =
      "https://app.ticketmaster.com/discovery/v2/events.json?apikey=rB4AA7GAeP49ivDzGWryLrRe8B6qMXl0&" +
      "keyword=" +
      searchInfo.keyword +
      "&segmentId=" +
      searchInfo.category +
      "&radius=" +
      searchInfo.distance +
      "&unit=" +
      searchInfo.unit +
      "&geoPoint=" +
      searchInfo.geoHash;

    https.get(myUrl, response => {
      const { statusCode } = response;
      const contentType = response.headers["content-type"];
      let error;
      if (statusCode !== 200) {
        error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
      }
      if (error) {
        console.error(error.message);
        res.status(200).json({
          message: "fail"
        });
        response.resume();
        return;
      }
      let rawData = "";
      response.on("data", chunk => {
        rawData += chunk;
      });
      response
        .on("end", () => {
          try {
            const parsedData = JSON.parse(rawData);
            res.status(200).json({
              message: "success",
              result: parsedData
            });
          } catch (e) {
            console.error(e.message);
            res.status(200).json({
              message: "fail"
            });
          }
        })
        .on("error", e => {
          console.log(`Got error: ${e.message}`);
          res.status(200).json({
            message: "fail"
          });
        });
    });
  } else {
    let location = searchInfo.geoCode.replace(/ /g, "+");
    let myUrl =
      "https://maps.googleapis.com/maps/api/geocode/json?address=" +
      location +
      "&key=AIzaSyAHzKMJe78M";

    https.get(myUrl, resGoogle => {
      const { statusCode } = resGoogle;
      const contentType = resGoogle.headers["content-type"];
      let error;
      if (statusCode !== 200) {
        error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
      }
      if (error) {
        console.error(error.message);
        res.status(200).json({
          message: "fail"
        });
        res.resume();
        return;
      }
      let rawData = "";
      resGoogle.on("data", chunk => {
        rawData += chunk;
      });
      resGoogle
        .on("end", () => {
          try {
            const parsedData = JSON.parse(rawData);
            geoCode = [
              parsedData.results[0].geometry.location.lat,
              parsedData.results[0].geometry.location.lng
            ];
            searchInfo.geoHash = geohash.encode(geoCode[0], geoCode[1], 5);

            let tmUrl =
              "https://app.ticketmaster.com/discovery/v2/events.json?apikey=rB7GAl0&" +
              "keyword=" +
              searchInfo.keyword +
              "&segmentId=" +
              searchInfo.category +
              "&radius=" +
              searchInfo.distance +
              "&unit=" +
              searchInfo.unit +
              "&geoPoint=" +
              searchInfo.geoHash;

            https.get(tmUrl, response => {
              const { statusCode } = response;
              const contentType = response.headers["content-type"];
              let error;
              if (statusCode !== 200) {
                error = new Error(
                  "Request Failed.\n" + `Status Code: ${statusCode}`
                );
              }
              if (error) {
                console.error(error.message);
                res.status(200).json({
                  message: "fail"
                });
                response.resume();
                return;
              }
              let rawData = "";
              response.on("data", chunk => {
                rawData += chunk;
              });
              response
                .on("end", () => {
                  try {
                    const parsedData = JSON.parse(rawData);
                    res.status(200).json({
                      message: "success",
                      result: parsedData
                    });
                  } catch (e) {
                    console.error(e.message);
                    res.status(200).json({
                      message: "fail"
                    });
                  }
                })
                .on("error", e => {
                  console.log(`Got error: ${e.message}`);
                  res.status(200).json({
                    message: "fail"
                  });
                });
            });
          } catch (e) {
            console.error(e.message);
            res.status(200).json({
              message: "fail"
            });
          }
        })
        .on("error", e => {
          console.log(`Got error: ${e.message}`);
          res.status(200).json({
            message: "fail"
          });
        });
    });
  }
});

app.get("/searchDetail", (req, res, next) => {
  const searchInfo = req.query;
  const myUrl = 'https://app.ticketmaster.com/discovery/v2/events/' + searchInfo.id + '.json?apikey=rB4yLrRe8B6qMXl0'
  https.get(myUrl, response => {
    const { statusCode } = response;
    const contentType = response.headers["content-type"];
    let error;
    if (statusCode !== 200) {
      error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
    }
    if (error) {
      console.error(error.message);
      res.status(200).json({
        message: "fail"
      });
      response.resume();
      return;
    }
    let rawData = "";
    response.on("data", chunk => {
      rawData += chunk;
    });
    response
      .on("end", () => {
        try {
          const parsedData = JSON.parse(rawData);
          res.status(200).json({
            message: "success",
            result: parsedData
          });
        } catch (e) {
          console.error(e.message);
          res.status(200).json({
            message: "fail"
          });
        }
      })
      .on("error", e => {
        console.log(`Got error: ${e.message}`);
        res.status(200).json({
          message: "fail"
        });
      });
  });
});

app.get("/searchSpotifyImg", (req, res, next) => {
  const info = req.query;
  let artist = {
    spotify: {},
    img: []
  }
  spotifyApi.clientCredentialsGrant().then(data => {
    // console.log("The access token is " + data.body["access_token"]);
    spotifyApi.setAccessToken(data.body["access_token"]);
  },
    function (err) {
      console.log('Something went wrong when retrieving an access token', err);
    }
  ).then(() => {
      spotifyApi.searchArtists(info.artist).then(data => {
        artist.spotify = data.body;
      }, err => {
        console.log("something wrong: " + err);
      }).then(() => {
        const myUrl = "https://www.googleapis.com/customsearch/v1?q=" + info.artist + "&cx=012947260702438846026:bbf3zkgy9xw&imgSize=huge&imgType=news&num=8&searchType=image&key=AIzaSyCUoNn_4eoxsrlaLpGzeI";

        https.get(myUrl, response => {
          const { statusCode } = response;
          const contentType = response.headers["content-type"];
          let error;
          if (statusCode !== 200) {
            error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
          }
          if (error) {
            console.error(error.message);
            res.status(200).json({
              message: "fail"
            });
            response.resume();
            return;
          }
          let rawData = "";
          response.on("data", chunk => {
            rawData += chunk;
          });
          response
            .on("end", () => {
              try {
                const parsedData = JSON.parse(rawData);
                artist.img = parsedData;
                res.status(200).json(artist);
              } catch (e) {
                console.error(e.message);
                res.status(200).json({
                  message: "fail"
                });
              }
            })
            .on("error", e => {
              console.log(`Got error: ${e.message}`);
              res.status(200).json({
                message: "fail"
              });
            });
        });
      });
    });  
});

app.get("/searchArtistImg", (req, res, next) => {
  const info = req.query;
  let artist = {
    spotify: {},
    img: []
  }
  
  const myUrl = "https://www.googleapis.com/customsearch/v1?q=" + info.artist + "&cx=012947260702438846026:bbf3zkgy9xw&imgSize=huge&imgType=news&num=8&searchType=image&key=AIzaSyCUoNn_4e9fXWCeI";

  https.get(myUrl, response => {
    const { statusCode } = response;
    const contentType = response.headers["content-type"];
    let error;
    if (statusCode !== 200) {
      error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
    }
    if (error) {
      console.error(error.message);
      res.status(200).json({
        message: "fail"
      });
      response.resume();
      return;
    }
    let rawData = "";
    response.on("data", chunk => {
      rawData += chunk;
    });
    response
      .on("end", () => {
        try {
          const parsedData = JSON.parse(rawData);
          artist.img = parsedData;
          res.status(200).json(artist);
        } catch (e) {
          console.error(e.message);
          res.status(200).json({
            message: "fail"
          });
        }
      })
      .on("error", e => {
        console.log(`Got error: ${e.message}`);
        res.status(200).json({
          message: "fail"
        });
      });
  });
})


app.get("/searchVenue", (req, res, next) => {
  const venueInfo = req.query;
  const myUrl = "https://app.ticketmaster.com/discovery/v2/venues?apikey=rB4AA7GAvB6qMXl0&keyword=" + venueInfo.name;
  https.get(myUrl, response => {
    const { statusCode } = response;
    const contentType = response.headers["content-type"];
    let error;
    if (statusCode !== 200) {
      error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
    }
    if (error) {
      console.error(error.message);
      res.status(200).json({
        message: "fail"
      });
      response.resume();
      return;
    }
    let rawData = "";
    response.on("data", chunk => {
      rawData += chunk;
    });
    response
      .on("end", () => {
        try {
          const parsedData = JSON.parse(rawData);
          res.status(200).json(parsedData);
        } catch (e) {
          console.error(e.message);
          res.status(200).json({
            message: "fail"
          });
        }
      })
      .on("error", e => {
        console.log(`Got error: ${e.message}`);
        res.status(200).json({
          message: "fail"
        });
      });
  });
});

app.get("/searchUpcoming", (req, res, next) => {
  const venueInfo = req.query;
  const myUrl = "https://api.songkick.com/api/3.0/search/venues.json?query=" + venueInfo.name + "&apikey=D7Um44C";
  https.get(myUrl, response => {
    const { statusCode } = response;
    const contentType = response.headers["content-type"];
    let error;
    if (statusCode !== 200) {
      error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
    }
    if (error) {
      console.error(error.message);
      res.status(200).json({
        message: "fail"
      });
      response.resume();
      return;
    }
    let rawData = "";
    response.on("data", chunk => {
      rawData += chunk;
    });
    response
      .on("end", () => {
        try {
          const venueId = JSON.parse(rawData)['resultsPage']['results']['venue'][0]['id'];
          const venueUrl = "https://api.songkick.com/api/3.0/venues/"+venueId+"/calendar.json?apikey=D7UA44C";
          https.get(venueUrl, venueRes => {
            const { statusCode } = venueRes;
            const contentType = venueRes.headers["content-type"];
            let error;
            if (statusCode !== 200) {
              error = new Error("Request Failed.\n" + `Status Code: ${statusCode}`);
            }
            if (error) {
              console.error(error.message);
              res.status(200).json({
                message: "fail"
              });
              venueRes.resume();
              return;
            }
            let rawData = "";
            venueRes.on("data", chunk => {
              rawData += chunk;
            });
            venueRes
              .on("end", () => {
                try {
                  const parsedData = JSON.parse(rawData);
                  res.status(200).json({
                      message: "success",
                      result: parsedData
                  });
                } catch (e) {
                  console.error(e.message);
                  res.status(200).json({
                    message: "fail"
                  });
                }
              })
              .on("error", e => {
                console.log(`Got error: ${e.message}`);
                res.status(200).json({
                  message: "fail"
                });
              });
          });
        } catch (e) {
          console.error(e.message);
          res.status(200).json({
            message: "fail"
          });
        }
      })
      .on("error", e => {
        console.log(`Got error: ${e.message}`);
        res.status(200).json({
          message: "fail"
        });
      });
  });
  
})

module.exports = app;
