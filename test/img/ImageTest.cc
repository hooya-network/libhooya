#include "ImageTest.hh"

TEST_F(ImageTest, Load) {
	hooya::img::Image i;
	i.FromFile(MAHORO_LANDSCAPE);

	ASSERT_EQ(i.Width(), 3200);
	ASSERT_EQ(i.Height(), 1200);

	i.FromFile(MAHORO_PORTRAIT);
	ASSERT_EQ(i.Width(), 1449);
	ASSERT_EQ(i.Height(), 2600);
}

TEST_F(ImageTest, CropSquare) {
	hooya::img::Image i;

	i.FromFile(MAHORO_LANDSCAPE);
	i.CropSquare();
	ASSERT_EQ(i.Width(), i.Height());

	i.FromFile(MAHORO_PORTRAIT);
	i.CropSquare();
	ASSERT_EQ(i.Width(), i.Height());
}

TEST_F(ImageTest, Resize) {
	hooya::img::Image i;

	i.FromFile(MAHORO_LANDSCAPE);
	/* Shrink */
	i.Resize(500);
	ASSERT_EQ(i.Width(), 500);
	/* Upscale */
	i.Resize(5000);
	ASSERT_EQ(i.Width(), 5000);

	/* Shrink */
	i.FromFile(MAHORO_PORTRAIT);
	i.Resize(500);
	ASSERT_EQ(i.Height(), 500);
	/* Upscale */
	i.Resize(5000);
	ASSERT_EQ(i.Height(), 5000);
}
