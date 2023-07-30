let dist = document.getElementById("dist");
let dist_inches = document.getElementById("dist-in");
let gas = document.getElementById("gas");
let gas_box = document.getElementById("gas-box");
let gas_s = document.getElementById("gas-s");
let temp = document.getElementById("temp");
let temp_f = document.getElementById("temp-f");
let hum = document.getElementById("hum");
let hum_s = document.getElementById("hum-s");

// Check to see when data updates on the website
const observer = new MutationObserver((mutations) => {
    for (mutation of mutations) {
        if (mutation.type === "childList") {
            // Change the background of the gas data box and the safety rating
            if (gas.textContent >= 750) {
                gas_box.style.backgroundColor = "lightcoral";
                gas_s.textContent = "Dangerous";
            }
            else if (gas.textContent >= 250) {
                gas_box.style.backgroundColor = "orange";
                gas_s.textContent = "Unsafe";
            }
            else {
                gas_box.style.backgroundColor = "lightgreen";
                gas_s.textContent = "Safe";
            }

            // Change the humidity rating
            if (hum.textContent > 60) {
                hum_s.textContent = "Humid";
            } else if (hum_s.textContent > 25) {
                hum_s.textContent = "Comfortable";
            } else {
                hum_s.textContent = "Dry";
            }

            // Convert distance from centimeters to inches rounded to 2 decimal places
            dist_inches.textContent = Math.round(dist.textContent / 0.0254) / 100;

            // Convert temperature from Celsius to fahrenheit rounded to 1 decimal place
            temp_f.textContent = Math.round(temp.textContent * 90 / 5) / 10 + 32;
        }
    }
})

observer.observe(gas, {attributes: true, characterData: true, subtree: true, childList: true});

// Slideshow code
let curr_slide = [0, 0];
let slide_arr = ["slides0", "slides1"];

// Show the first slide of every slideshow
for (let s = 0; s < slide_arr.length; s++) {
    chooseSlide(0, s);
}

function switchSlide(d, n) {
    renderSlides(curr_slide[n] + d, n);
}

function chooseSlide(index, n) {
    renderSlides(index, n);
}

function renderSlides(index, n) {
    let slides = document.getElementsByClassName(slide_arr[n]);

    curr_slide[n] = index;

    if (curr_slide[n] === slides.length) {
        curr_slide[n] = 0;
    } else if (curr_slide[n] < 0) {
        curr_slide[n] = slides.length - 1;
    }

    let i;
    for (i = 0; i < slides.length; i++) {
        slides[i].style.display = "none";
    }

    slides[curr_slide[n]].style.display = "block";
}