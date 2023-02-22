function App() {  

  let scale = 3;

  return (
    <div className="flex flex-row w-full h-screen justify-center bg-neutral-900">
      <div className="flex flex-col h-full justify-center">
        <canvas id="canvas" height={144 * scale} width={160 * scale}></canvas>
      </div>
    </div>
  );
}

export default App;
