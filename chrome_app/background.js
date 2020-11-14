chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('tof_image.html', {
    'outerBounds': {
      'width': 500,
      'height': 500
    }
  });
});
