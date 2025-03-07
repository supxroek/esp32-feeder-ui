export async function handler() {
  const response = await fetch("http://192.168.32.16/feed");
  const data = await response.json();
  return {
    statusCode: 200,
    body: JSON.stringify(data),
  };
}
